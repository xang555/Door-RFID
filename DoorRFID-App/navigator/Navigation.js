import { StackNavigator } from 'react-navigation';
import LoginScreen from '../layouts/login';
import MainScreen from '../layouts/main';
import SplashScreen from '../layouts/splashScreen';

const AppNavigator = StackNavigator({
    Login:{ screen: LoginScreen },
    Main : { screen: MainScreen },
    SplashScreen: { screen: SplashScreen}
},{ headerMode: 'screen' });

export default AppNavigator;