const express = require('express');
const app = express();
const server = require('http').createServer(app);
const WebSocket = require('ws');

app.use(express.json());

const wss = new WebSocket.Server({ server:server });
let volunteerSocket;
let resolvePending = null;

wss.on('connection', function connection(ws) {
    console.log('A new client has connected.');

    volunteerSocket = ws;

    ws.on('message', function incoming(message) {
        console.log("Reponse received from volunteer.");

        if (resolvePending) {
            resolvePending(message.toString());
            resolvePending = null;
        }
    });
});

app.post('/appointment', async function(req, res){
    if (!volunteerSocket || volunteerSocket.readyState !== WebSocket.OPEN) {
        return res.status(500).send('Client 2 not connected');
    }

    let user_name = req.body.first_name + " " + req.body.last_name;
    let time_string;
    switch (req.body.time) {
        case 0:
            time_string = "morning";
            break;
        case 1:
            time_string = "afternoon";
            break;
        case 2:
            time_string = "evening";
            break;
        default:
            time_string = "night"
    }
    let day_string;
    switch(req.body.time) {
        case 0:
            day_string = "today";
            break;
        case 1:
            day_string = "tomorrow";
            break;
        default:
            day_string = "sometime this week";
    }

    volunteerSocket.send(`Request received.\nUser: ${user_name}.\nCooking: ${req.body.cooking ? "yes" : "no"}.\nShopping: ${req.body.shopping ? "yes" : "no"}.\nChildcare: ${req.body.childcare ? "yes" : "no"}.\nEmotional support: ${req.body.emotional_support ? "yes" : "no"}.\nLaundry: ${req.body.laundry ? "yes" : "no"}.\nCleaning: ${req.body.cleaning ? "yes" : "no"}.\nGardening: ${req.body.gardening ? "yes" : "no"}.\nRepair: ${req.body.repair ? "yes" : "no"}.\nWhen: ${day_string}, during the ${time_string}.\nDo you accept this request?`);

    const responseFromClient2 = await new Promise((resolve) => {
        resolvePending = resolve;
    });

    res.json({ accepted: responseFromClient2 == "yes" ? 1 : 0,  first_name: "Ava", last_name: "Fischer"});
});

server.listen(3000, () => console.log("Listening on port: 3000"));
