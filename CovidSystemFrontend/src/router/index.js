import Vue from "vue";
import Router from "vue-router";
import Home from "../views/Home";
import MapView from "../views/MapView";
import Hospital from "../views/Hospital";
import PeopleRoute from "../views/PeopleRoute";
import RiskArea from "../views/RiskArea";

Vue.use(Router)
export default new Router(
    {
        mode : 'history',
        routes : [
            {
                path : '/',
                name : 'Home',
                component : Home,
                children : [
                    {
                        path : '/map',
                        name : 'MapView',
                        component : MapView
                    },
                    {
                        path : '/hospital',
                        name : 'Hospital',
                        component : Hospital
                    },
                    {
                        path : '/route',
                        name : 'PeopleRoute',
                        component : PeopleRoute
                    },
                    {
                        path : '/riskArea',
                        name : 'riskArea',
                        component : RiskArea
                    }
                ]
            }
        ]
    }
)