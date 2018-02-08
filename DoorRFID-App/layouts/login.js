import React,{ Component } from 'react';
import { FormLabel, FormInput, FormValidationMessage, Button, Text } from 'react-native-elements'
import { View, StyleSheet,Alert } from 'react-native';
import { connect } from 'react-redux';
import { login } from '../actions/LoginAction';
import { bindActionCreators } from 'redux';
    
class LoginScreen extends Component {

    static navigationOptions = {
        header: null
      }

      constructor(props) {
          super(props);
          this.state={
              uname : "",
              passwd : ""
          }
      }

    _handleLogin = () => {

        const { uname,passwd } = this.state;
        if(uname.trim().length > 0 && passwd.trim().length > 0){
            this.props.login(uname,passwd);
        }else {
            Alert.alert("Door-RFID","Please, Enter Correct Username and Passworfd");
        }

    }  

    render(){

        const { loginState } = this.props;

        return (

            <View style={styls.container}>

             <View style={{ marginBottom:16 }}>
              <Text style={styls.message}>WelCome, Please Login</Text>
              <Text style={styls.msgDiscussion}>Please, Connect Door Wifi befor do anything</Text>      
            </View>

             <FormInput 
              inputStyle={styls.textInput} 
              placeholder="UserName"
              onChangeText={(val) =>  this.setState({uname : val}) }/>

             <FormInput
              inputStyle ={styls.textInput}
              placeholder="Password" 
              secureTextEntry={true} 
              onChangeText={(val) =>  this.setState({passwd : val}) }/>

             <Button 
              onPress={this._handleLogin}  
              loading = {loginState.is_loging}
              backgroundColor="#00E676"
              borderRadius={5}
              containerViewStyle={styls.button}
              title="Login" />

                {
                    loginState.is_failed ? <Text style={styls.msgError}>Login failed, Please try again</Text> : null   
                }

            </View>    

        );

    }


}

const styls = StyleSheet.create({
    
        container : {
            flex: 1,
            flexDirection: 'column',
            justifyContent: 'center',
            alignItems: 'center'
        },
        message : {
            fontSize: 27,
            textAlign:'center',
            color: "#000000"
        },
        textInput: {
            width: 250,
            height: 50
        },
        button: {
            width:250,
            borderWidth:1,
            borderColor:"white",
            marginTop:16
        },
        msgDiscussion : {
            width:300,
            fontSize:12,
            textAlign: "center"
        },
        msgError : {
            color:"#ff005f",
            width:250,
            fontSize:18,
            textAlign: "center",
            marginTop:16
        }
    
    });


const mapStateToPtops = (state) => ({
    loginState : state.login
});

const mapDispatchToProps = (dispatch) => {
    return bindActionCreators({login},dispatch);
};

export default connect(mapStateToPtops,mapDispatchToProps)(LoginScreen);

