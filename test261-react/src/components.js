import React from 'react';

export class MyButton extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      accepting: false,
      clicking: false
    };
  }

  computeClass() {
    return 'button'
      + (this.state.accepting ? ' accepting' : '')
      + (this.state.clicking ? ' clicking' : '');
  }

  render() {
    // Manually implement :hover and :active.
    return (<span className={this.computeClass()}
                  onMouseEnter={_ => this.setState({ accepting: true, clicking: false })}
                  onMouseLeave={_ => this.setState({ accepting: false, clicking: false })}
                  onMouseDown={_ => this.setState({ clicking: true })}
                  onMouseUp={_ => this.setState({ clicking: false })}>
      {this.props.label}
    </span>);
  }
}
