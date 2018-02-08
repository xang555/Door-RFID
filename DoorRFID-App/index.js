import React from 'react';
import { AppRegistry } from 'react-native';
import App from './App';
import { Provider } from 'react-redux';
import store from './store';

const RootApp = () => {
    return (
        <Provider store= { store }>
            <App/>
        </Provider>    
    );
}    

AppRegistry.registerComponent('DoorRFIDApp', () => RootApp);
