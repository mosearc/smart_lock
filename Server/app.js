const express = require("express");
const { Router } = express;
const fs = require("fs");


/*
 * Simple json database disk save/load handler
 */
class Database {
    constructor(path) {
        this.path = path;
        this.data = {};
    }

    load() {
        if(!fs.existsSync(this.path))
            return;
        
        this.data = JSON.parse(fs.readFileSync(this.path));
        console.log("Loading: ", this.data);
    }

    save() {
        fs.writeFileSync(this.path, JSON.stringify(this.data, null, 2));
        console.log("Saving: ", this.data);
    }

    getCardByCardIdPhysical(cardIdPhysical) {
        if(this.data.cards === undefined)
            return undefined;
        return Object.values(this.data.cards).find(obj => {
            return obj.cardIdPhysical === cardIdPhysical;
        });
    }
};

const app = express();
const db = new Database("./db.json");
db.load();

//Handle lock authentication through (lockId, apiKey) credentials
const lockParser = (req, res, next) => {
    req.lock = undefined;

    if(db.data.locks === undefined) {
        next();
        return;
    }

    const lockId = req.query["lockId"];
    const apiKey = req.headers["api-key"];
    const lock = db.data.locks[lockId];

    if(lock !== undefined && lock.apiKey === apiKey) {
        req.lock = lock;
    }

    next();
};
app.use(lockParser);
//Require apiKey for all requests (for now)
const requireApiKey = (req, res, next) => {
    if(req.lock === undefined) {
        res.status(401).send("Unauthorized Api-Key");
    } else {
        next();
    }
};
app.use(requireApiKey);

//Simple status data object
const Status = (result, message) => {
    return {
        result: result,
        message: message
    };
};

//Check if card corresponding to specific cardIdPhysical is authorized to open the lock
const isAuthorized = (req) => {
    const lock = req.lock;
    const cardIdPhysical = req.query["cardIdPhysical"];
    const pin = req.query["pin"];

    if(cardIdPhysical === undefined) {
        return false;
    }

    const card = db.getCardByCardIdPhysical(cardIdPhysical);
    let authorized = card !== undefined && card.active !== undefined && card.active && ((card.locks !== undefined && card.locks.includes(lock.lockId)) || (card.role !== undefined && card.role.split(" ").includes("passepartout")));
    
    //PIN handling through the server already implemented server side. Could used MPU side too, in the future
    if(pin !== undefined) {
        authorized = authorized && card.pin !== undefined && pin === card.pin;
    }

    return authorized;
};

//Lock blocking/unblocking handling through the server already implemented server side. Could used MPU side too, in the future
const lockRouter = Router();
lockRouter.get("/block", (req, res) => {
    req.lock.blocked = true;
    db.save();
    res.json(Status("true", "Lock blocked"));
});
lockRouter.get("/unblock", (req, res) => {
    const cardIdPhysical = req.query["cardIdPhysical"];
    const puk = req.query["puk"];
    let unblock = false;

    if(cardIdPhysical !== undefined) {
        const card = db.getCardByCardIdPhysical(cardIdPhysical);
        unblock = isAuthorized(req) && card !== undefined && card.role !== undefined && card.role.split(" ").includes("master");
    } else if(puk !== undefined) {
        unblock = req.lock.puk !== undefined && puk === req.lock.puk;
    }

    if(unblock) {
        req.lock.blocked = false;
        db.save();
        res.json(Status("true", "Lock unblocked"));
        return;
    }

    res.json(Status("false", "Invalid cardIdPhysical or puk"));
});
lockRouter.get("/isblocked", (req, res) => {
    if(req.lock.blocked === true)
        res.json(Status("true", "Lock is blocked"));
    else
        res.json(Status("false", "Lock is not blocked"));
});
app.use("/lock", lockRouter);

const cardRouter = Router();
//Handles card authentication checking requests from the MPU client
cardRouter.get("/isauthorized", (req, res) => {
    let authorized = !req.lock.blocked && isAuthorized(req);
    res.json(Status(authorized ? "true" : "false", authorized ? "Authorized" : "Not authorized"));
});
app.use("/card", cardRouter);

//Handle 404
app.all("*", (req, res) => {
    res.sendStatus(404).end();
});



//Change listening ip and port as you need
const info = app.listen(3000, "127.0.0.1", () => {
    console.log("Listening on " + info.address().address + ":" + info.address().port);
});
