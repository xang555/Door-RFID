import React,{ Component } from 'react';
import { View, StyleSheet } from 'react-native';
import { connect } from 'react-redux';
import { _getToken } from '../dbManager';
import { NavigationActions } from 'react-navigation';


export default class SplashScreen extends Component {

    static navigationOptions = {
        header: null
      }

    _navigateTo = (routeName) => {
        const resetAction = NavigationActions.reset({
          index: 0,
          actions: [NavigationActions.navigate({ routeName })]
        })
        this.props.navigation.dispatch(resetAction)
      }

    componentDidMount(){
        _getToken().then(token => {
            if(token !== null && token.trim().length > 0) {
              this._navigateTo('Main');
            }else{
                this._navigateTo('Login');
            }
          }).catch(err => {
            console.log(err);
            this._navigateTo('Login');
          });
    }


    render() {
        return (
            <View  style={styles.container}/>
        )
    }


}

const styles = StyleSheet.create({
    container : {
        flex: 1,
        backgroundColor: '#F5FCFF'
    }
});