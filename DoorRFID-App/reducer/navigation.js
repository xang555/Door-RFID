import Navigator from '../navigator/Navigation';

const initialState = Navigator.router.getStateForAction(Navigator.router.getActionForPathAndParams("SplashScreen"));

const navReducer = (state = initialState, action) => {
    const nextState = Navigator.router.getStateForAction(action, state);
    return nextState || state;
  };

  export default navReducer;