"use strict";

// https://developers.google.com/cast/docs/reference/chrome/

class CastApp {
  constructor() {
  }

  // Configures GCast API with this application instance.
  configureCastAPI() {
    const applicationID = chrome.cast.media.DEFAULT_MEDIA_RECEIVER_APP_ID;

    const sessionRequest = new chrome.cast.SessionRequest(applicationID);
    const apiConfig = new chrome.cast.ApiConfig(
      sessionRequest,
      this.sessionListener.bind(this),
      this.receiverListener.bind(this)
    );

    chrome.cast.initialize(
      apiConfig,
      () => console.log("Initialization: OK"),
      (error) => console.error("Initialization:", error)
    );
  }

  // Handles GCast API session event. The event notifies the app a hanging
  // session on initialization.
  sessionListener(session) {
    this.setSession(session);
  }

  // Handles GCast API receiver event.
  receiverListener(receiverAvailability) {
    if (receiverAvailability == chrome.cast.ReceiverAvailability.AVAILABLE) {
      this.enableConnectButton();
    }
  }

  enableConnectButton() {
    const connectButton = document.getElementById("connect-btn");

    connectButton.addEventListener("click", event => this.requestSession());
    connectButton.disabled = false;
  }

  // Requests a cast session. This opens the modal cast widget.
  requestSession() {
    chrome.cast.requestSession(
      session => this.setSession(session),
      error => console.error(error)
    );
  }

  // Sets app's session to a cast receiver.
  setSession(session) {
    this.session = session;
    this.enableCastButton();

    console.log("Session:", session);
  }

  enableCastButton() {
    const castButton = document.getElementById("cast-btn");

    castButton.addEventListener("click", event => {
      const mediaURLInput = document.getElementById("media-url");
      this.castURL(mediaURLInput.value);
    });
    castButton.disabled = false;
  }

  // Plays a video on the cast receiver.
  castURL(url) {
    console.log("Media:", url);

    const contentType = this.deduceContentType(url);
    const mediaInfo = new chrome.cast.media.MediaInfo(url, contentType);
    const request = new chrome.cast.media.LoadRequest(mediaInfo);

    console.log("Request:", request);

    this.session.loadMedia(
      request,
      media => console.log("Media:", media),
      error => console.error(error)
    );
  }

  deduceContentType(url) {
    const extMatch = url.match(/\.(.*?)$/);
    const ext = (extMatch ? extMatch[1] : "");

    const TYPE_MAP = {
      webm: "video/webm",
      mkv: "video/webm",
      mp4: "video/mp4"
    };
    const DEFAULT_CONTENT_TYPE = "video/webm";

    return TYPE_MAP.hasOwnProperty(ext) ? TYPE_MAP[ext] : DEFAULT_CONTENT_TYPE;
  }
}

const castApp = new CastApp();

window.__onGCastApiAvailable = isAvailable => {
  if (isAvailable) {
    castApp.configureCastAPI();
  }
};
