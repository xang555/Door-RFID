import { combineReducers } from 'redux';
import navReducer from './navigation';
import LoginReducer from './login';

const rootReducer = combineReducers({
    nav : navReducer,
    login : LoginReducer
});

export default rootReducer;