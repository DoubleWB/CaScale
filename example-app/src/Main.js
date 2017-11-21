import React, { Component } from "react";
import {
  Route,
  NavLink,
  HashRouter
} from "react-router-dom";
import Home from "./Home";
import Stuff from "./Stuff";
import Contact from "./Contact";
import Recipes from "./Recipes";
function Box(props) {
  return (
    <div className = 'box'>
      {props.children}
    </div>
  );
}

class Login extends React.Component {
  constructor(props){
      super(props);
      this.state = {user: '', pass: ''};

      this.handleUser = this.handleUser.bind(this);
      this.handlePass = this.handlePass.bind(this);
      this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleUser(event) {
    this.setState({user: event.target.value});
  }

  handlePass(event) {
    this.setState({pass: event.target.value});
  }

  handleSubmit(event) {
    event.preventDefault();
  }

  render() {
    return (
      <Box>
      <div className = "login">
      <form onSubmit = {this.handleSubmit} class = "form-group">
          Username:
          <input type="text" value = {this.state.user} onChange = {this.handleUser} />
           <br></br>
          Password:
          <input type="password" value = {this.state.pass} onChange = {this.handlePass} />
      <input type="submit" value = "Submit" />
      </form>
      </div>
      </Box>
    );
  } 
}

class App extends Component {
  render() {
    return (
      <div className="App">
      <header className="App-header">
        <img src="logo.svg" className="App-logo" alt="logo" />
        <h1 className="App-title">Welcome to CaScale</h1>
      </header>
      <body>
      <Login></Login>
      </body>
      </div>
    );
  }
}

class Main extends Component {
  render() {
    return (
      <HashRouter>
        <div>
          <h1>CaScale</h1>
          <ul className="header">
            <li><NavLink exact to="/">Home</NavLink></li>
            <li><NavLink to="/stuff">Login</NavLink></li>
            <li><NavLink to="/contact">Recipes</NavLink></li>
          </ul>
          <div className="content">
            <Route exact path="/" component={Home}/>
            <Route path="/stuff" component={Login}/>
            <Route path="/contact" component={Recipes}/>
          </div>
        </div>
      </HashRouter>
    );
  }
}

export default Main;