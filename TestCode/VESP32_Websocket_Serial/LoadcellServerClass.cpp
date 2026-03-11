#include "LoadcellServerClass.h"

// HTML UI - Tách biệt hoàn toàn
const char LoadcellServer::INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><title>Loadcell Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1"><meta charset="utf-8">
<style>
    body { font-family: sans-serif; text-align: center; background: #ececec; }
    .card { max-width: 400px; margin: 20px auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
    .display { background: #222; color: #0f0; padding: 15px; border-radius: 5px; margin-bottom: 15px; }
    #gt1 { font-size: 2.5em; font-weight: bold; }
    #gt2 { color: #888; font-size: 0.9em; }
    .grid { display: grid; grid-template-columns: 1fr 1fr; gap: 8px; }
    button { padding: 12px; border: none; border-radius: 5px; cursor: pointer; font-weight: bold; }
    .full { grid-column: span 2; }
    .btn-red { background: #e74c3c; color: white; }
    .btn-blue { background: #3498db; color: white; }
    .btn-green { background: #27ae60; color: white; }
</style>
</head><body>
<div class="card">
    <h2>LOADCELL UI</h2>
    <div class="display">
        <div id="gt1">0.00</div>
        <div id="gt2">Raw: 0</div>
    </div>
    <div class="grid">
        <button class="btn-red full" onclick="s('TARE')">TARE</button>
        <button class="btn-blue full" onclick="s('ROUND')">ROUND</button>
        <button class="btn-green" onclick="s('SET_1')">Sample x1</button>
        <button class="btn-green" onclick="s('SET_5')">Sample x5</button>
        <button class="btn-green" onclick="s('SET_10') Bryan">Sample x10</button>
        <button class="btn-green" onclick="s('SET_20')">Sample x20</button>
        <button class="btn-green full" onclick="s('SET_25')">Sample x25</button>
    </div>
</div>
<script>
    var ws = new WebSocket('ws://' + window.location.hostname + '/ws');
    ws.onmessage = function(e) {
        var d = JSON.parse(e.data);
        document.getElementById('gt1').innerHTML = d.gt1;
        document.getElementById('gt2').innerHTML = 'Raw: ' + d.gt2;
    };
    function s(m) { ws.send(m); }
</script>
</body></html>
)rawliteral";

static LoadcellServer* instance = nullptr; 

LoadcellServer::LoadcellServer(uint16_t port) : _server(port), _ws("/ws") {
    instance = this;
}

void LoadcellServer::begin(ButtonCallback cb) {
    _callback = cb;

    _ws.onEvent(onEvent);
    _server.addHandler(&_ws);

    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", INDEX_HTML);
    });

    _server.begin();
}

void LoadcellServer::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                             AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_DATA && instance->_callback) {
        String msg = "";
        for(size_t i=0; i<len; i++) msg += (char)data[i];
        instance->_callback(msg); // Gọi callback về main
    }
}

void LoadcellServer::updateData(float gt1, long gt2) {
    if (_ws.count() > 0) {
        String json = "{\"gt1\":" + String(gt1, 2) + ",\"gt2\":" + String(gt2) + "}";
        _ws.textAll(json);
    }
}

void LoadcellServer::cleanup() {
    _ws.cleanupClients();
}