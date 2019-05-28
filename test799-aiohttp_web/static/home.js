(function() {
  "use strict";

  fetch("/status")
    .then(response => response.json())
    .then(response => {
      let { message } = response;
      let statusTag = document.getElementById("status");
      while (statusTag.firstChild !== null) {
        statusTag.removeChild(statusTag.firstChild);
      }
      statusTag.appendChild(document.createTextNode(message));
    });
})();
