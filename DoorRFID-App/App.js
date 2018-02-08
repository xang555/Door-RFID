/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import SplashScreen from 'react-native-splash-screen'
import AppWithNavigationState from './navigator/AppWithNavigation';
import { _getToken } from './dbManager';
import { NavigationActions } from 'react-navigation';
import { connect } from 'react-redux';

class App extends Component {

  componentDidMount(){
    setTimeout(() => {
      SplashScreen.hide();
    }, 2000);
  }

  render() {
    return (
        <AppWithNavigationState/>
    );
  }
  
}

  const mapDispatchToProps = (dispacth) => ({
    MainScreen : () => {
      dispacth(NavigationActions.reset({
        index: 0,
        actions: [NavigationActions.navigate({ routeName : 'Main' })]
      }));
    }

  });

export default connect(null,mapDispatchToProps)(App);