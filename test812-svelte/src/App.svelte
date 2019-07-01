<script>
  export let message;
</script>

<style>
  h1 {
    color: purple;
  }
</style>

<h1>{message}</h1>
<input bind:value="{message}">
