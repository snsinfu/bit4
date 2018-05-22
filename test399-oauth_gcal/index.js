document.addEventListener('DOMContentLoaded', function() {
  'use strict';

  var DISCOVERY_DOCS = ['https://www.googleapis.com/discovery/v1/apis/tasks/v1/rest'];
  var SCOPES = 'https://www.googleapis.com/auth/tasks.readonly';

  var authorizeButton = document.getElementById('authorize-button');
  var signoutButton = document.getElementById('signout-button');

  loadClient();

  function loadClient() {
    gapi.load('client:auth2', initClient);
  }

  function initClient() {
    gapi.client.init({
      apiKey:        API_KEY,
      clientId:      CLIENT_ID,
      discoveryDocs: DISCOVERY_DOCS,
      scope:         SCOPES
    }).then(function () {
      gapi.auth2.getAuthInstance().isSignedIn.listen(updateSigninStatus);
      updateSigninStatus(gapi.auth2.getAuthInstance().isSignedIn.get());

      authorizeButton.onclick = handleAuthClick;
      signoutButton.onclick = handleSignoutClick;
    });
  }

  function updateSigninStatus(isSignedIn) {
    if (isSignedIn) {
      authorizeButton.style.display = 'none';
      signoutButton.style.display = 'block';
      listTaskLists();
    } else {
      authorizeButton.style.display = 'block';
      signoutButton.style.display = 'none';
    }
  }

  function handleAuthClick(event) {
    gapi.auth2.getAuthInstance().signIn({
      prompt: 'select_account'
    });
  }

  function handleSignoutClick(event) {
    gapi.auth2.getAuthInstance().signOut();
  }

  function appendPre(message) {
    var pre = document.getElementById('content');
    var textContent = document.createTextNode(message + '\n');
    pre.appendChild(textContent);
  }

  function listTaskLists() {
    gapi.client.tasks.tasklists.list({
        maxResults: 10
    }).then(function(response) {
      appendPre('Task Lists:');
      var taskLists = response.result.items;
      if (taskLists && taskLists.length > 0) {
        for (var i = 0; i < taskLists.length; i++) {
          var taskList = taskLists[i];
          appendPre(taskList.title + ' (' + taskList.id + ')');
        }
      } else {
        appendPre('No task lists found.');
      }
    });
  }
});
