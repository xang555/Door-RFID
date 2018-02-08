
import { API_URL } from '../app.conf';
import { NavigationActions } from 'react-navigation';

export const START_LOGIN = "start_login";
export const LOGIN_SUCCESS = "login_success";
export const LOGIN_FAILED = "login_failed";

export function startLogin(){
    return {
        type : START_LOGIN
    }
}

export function loginSuccess(token){
    return {
        type : LOGIN_SUCCESS,
        token
    }
}

export function loginFailed() {
    return {
        type : LOGIN_FAILED
    }
}

export function login($user,$passwd) {
    return dispatch => {

        dispatch(startLogin());

        fetch(API_URL +"/login",{
            method:'POST',
            headers:{
                'Content-Type': 'application/x-www-form-urlencoded',
                "User-Agent":"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.13; rv:58.0) Gecko/20100101 Firefox/58.0"    
            },
            body : "uname="+$user+"&passwd="+$passwd
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
            if(parseInt(data.err) === 0){
                dispatch(loginSuccess(data.token));
                dispatch(NavigationActions.reset({
                    index : 0,
                    actions:[NavigationActions.navigate({routeName:'Main'})]
                }));
            }else {
                dispatch(loginFailed());    
            }
          }).catch(error => {
              dispatch(loginFailed());
          })  
    }
}