import React from 'react';
import ReactDOM from 'react-dom';

import { MyButton } from './components.js';

document.addEventListener('DOMContentLoaded', function() {
  ReactDOM.render(
    React.createElement(MyButton, { label: 'Click' }),
    document.getElementById('app')
  );
});
