import App from "./App.svelte";

const app = new App({
  target: document.getElementById("main"),
  props: {
    message: "Hello, world!"
  }
});

export default app;
