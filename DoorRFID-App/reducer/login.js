import { START_LOGIN,LOGIN_SUCCESS,LOGIN_FAILED } from '../actions/LoginAction';
import { _setToken } from '../dbManager';

const InitState = {
    is_loging : false,
    is_logedin : false,
    is_failed : false
}

export default function LoginReducer(state = InitState, action) {

    switch(action.type){
        case START_LOGIN :
            return Object.assign({},state,{
               is_loging : true 
            });
            break;
        case LOGIN_SUCCESS :
            _setToken(action.token);
            return Object.assign({},state,{
                is_loging : false,
                is_logedin : true,
                is_failed : false 
            });
            break;
        case LOGIN_FAILED :
            return Object.assign({},state,{
                is_loging : false,
                is_logedin : false,
                is_failed : true 
            });

            break;
    }

    return state;

}