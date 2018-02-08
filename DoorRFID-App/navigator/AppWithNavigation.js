import React, { Component } from 'react';
import { addNavigationHelpers } from 'react-navigation';
import AppNavigator from './Navigation';
import { connect } from 'react-redux';

class AppWithNavigationState extends Component {

    render(){
        return (
            <AppNavigator navigation={addNavigationHelpers({
                dispatch: this.props.dispatch,
                state : this.props.nav
            })}/>
        )
    }

}

const mapStateToProps = (state) => ({
    nav: state.nav
}); 

export default connect(mapStateToProps)(AppWithNavigationState);