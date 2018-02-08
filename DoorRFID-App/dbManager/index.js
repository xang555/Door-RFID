import { AsyncStorage } from 'react-native';

export const LOGIN_TOKEN = "@DoorRfidStore:token";

export async function _setToken($token) {
    try{
        await AsyncStorage.setItem(LOGIN_TOKEN,$token);
    }catch(err) {
        console.log(err);
    }
}

export async function _getToken(){
    try {
       let token = await AsyncStorage.getItem(LOGIN_TOKEN);
       if(token !== null) {
           return token;
       }
    }catch(err) {
        console.log(err);
    }
    return null;
}

export async function _removeToken() {
    try {
        await AsyncStorage.removeItem(LOGIN_TOKEN);
    }catch(err){
        console.log(err);
    }
}