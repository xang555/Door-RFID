import React,{ Component } from 'react';
import {  Button, Text } from 'react-native-elements'
import { View, StyleSheet,Alert } from 'react-native';
import { _getToken,_removeToken } from '../dbManager';
import { NavigationActions } from 'react-navigation';
import {  API_URL } from '../app.conf';

class MainScreen extends Component {

    static navigationOptions = ({navigation}) => ({
        title : "Door-RFID",
        headerRight : (<Button backgroundColor="transparent" color="green" title="Logout" onPress={() => { 
           
            Alert.alert("Door-RFID","Do you want to logout!?",[
                { text:'OK',onPress:() => {   
                    _removeToken();
                    navigation.dispatch(NavigationActions.reset({
                        index: 0,
                        actions:[NavigationActions.navigate({ routeName: 'Login' })]
                    }));

                 } },
                 { text:'NO', onPress:() => {  return; } }
            ]);

          }} />)
    });

    constructor(props) {
        super(props);
        this.state ={
            is_sending : false
        }
    }

    _handleSendCommand = ($token) => {
        this.setState({
            is_sending : true
        });
        fetch(API_URL +"/cmd",{
            method:'POST',
            headers:{
                'Content-Type': 'application/x-www-form-urlencoded',
                "User-Agent":"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.13; rv:58.0) Gecko/20100101 Firefox/58.0",
                "token": $token   
            },
            body : "cmd=" + 1
        }).then(res => {
            if(res.status === 200) {
                return res;
            }else {
                var error = new Error(response.statusText)
                error.response = response
                throw error
            }
        }).then(res => res.json())
          .then(data => {
            this.setState({
                is_sending : false
            });
          }).catch(error => {
            this.setState({
                is_sending : false
            });  
            Alert.alert("Door-RFID","Open Door Failed, Please check if you connected to the door of wifi");
          });

    }


    render() {
        return (
            
         <View style={styls.container}>

          <Button
          onPress={(e) => { 

            _getToken().then(token => {
                if(token !== null && token.trim().length > 0){
                    this._handleSendCommand(token);
                }else {
                    Alert.alert("Door-RFID","Please, Login Again");
                }
            }).catch(err => {
                Alert.alert("Door-RFID","Please, Login Again");
            });

        }}
          backgroundColor="#00E676"
          buttonStyle={styls.button}
          borderRadius={127}
          containerViewStyle={{ borderWidth:5, borderColor:"#ffffff" }}
          loading={this.state.is_sending}
          disabled = {this.state.is_sending}
          disabledStyle={{ backgroundColor:"#1B5E20" }}
          icon={ this.state.is_sending ? null : {name: 'key', type: 'foundation',size:80}}
          /> 

         </View>          

        );
    }

}

const styls = StyleSheet.create({

    button : {
        width: 250,
        height: 250,
        shadowOpacity: 0.75,
        shadowRadius: 5,
        shadowColor: 'green',
        shadowOffset: { height: 0, width: 0 }
    },
    container : {
        justifyContent: "center",
        alignItems: "center",
        flex: 1,
        flexDirection: "column"
    }

});


export default MainScreen;