var Cu = Components.utils;
Cu.import("resource://gre/modules/devtools/dbg-client.jsm");
Cu.import("resource://gre/modules/devtools/dbg-server.jsm");
const {require} = Cu.import("resource://gre/modules/devtools/Loader.jsm", {});

const Services = require("Services");

// Always log packets when running tests.
Services.prefs.setBoolPref("devtools.debugger.log", true);
Services.prefs.setBoolPref("dom.mozBrowserFramesEnabled", true);

SimpleTest.registerCleanupFunction(function() {
  Services.prefs.clearUserPref("devtools.debugger.log");
  Services.prefs.clearUserPref("dom.mozBrowserFramesEnabled");
});

const {promiseInvoke} = require("devtools/async-utils");

const { DirectorRegistry,
        DirectorRegistryFront } = require("devtools/server/actors/director-registry");

const { DirectorManagerFront } = require("devtools/server/actors/director-manager");

const {Task} = require("resource://gre/modules/Task.jsm");

/***********************************
 *  director helpers functions
 **********************************/

function* newConnectedDebuggerClient(opts) {
  var transport = DebuggerServer.connectPipe();
  var client = new DebuggerClient(transport);

  yield promiseInvoke(client, client.connect);

  var root = yield promiseInvoke(client, client.listTabs);

  return {
    client: client,
    root: root,
    transport: transport
  };
}

function purgeInstalledDirectorScripts() {
  DirectorRegistry.clear();
}