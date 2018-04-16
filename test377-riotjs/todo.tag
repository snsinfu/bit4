<todo>
  <form onsubmit={ add }>
    <input ref="input">
    <button>Add</button>
  </form>

  <todo-list tasks={ tasks } />

  this.tasks = [];

  add(event) {
    event.preventDefault();

    this.tasks.push({
      date: new Date(Date.now()),
      subject: this.refs.input.value
    });

    this.refs.input.value = '';
  }
</todo>

<todo-list>
  <ul>
    <todo-item each={ opts.tasks } task={ this } />
  </ul>
</todo-list>

<todo-item>
  <li>{ opts.task.date } - { opts.task.subject }</li>

  <style>
    li {
      transition: all 0.3s ease-out;
      opacity: 0;
    }

    .show {
      opacity: 1;
    }
  </style>

  this.on('mount', function() {
    var li = this.root.getElementsByTagName('li')[0];
    window.setTimeout(function() { li.classList.add('show'); }, 10);
  });
</todo-item>
