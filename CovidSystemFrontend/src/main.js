import Vue from 'vue'
import App from './App.vue'
import echarts from 'echarts'
import 'echarts/map/js/china'
import 'echarts/map/js/province/jilin'
import ElementUI from "element-ui"
import 'element-ui/lib/theme-chalk/index.css';
import router from "./router/index";
import axios from "axios";


Vue.prototype.$axios = axios.create({
  baseURL:"/api",
  headers:{'Content-Type':'application/json;charset=utf-8'},    // 设置传输内容的类型和编码
  withCredentials:true      // 指定某个请求应该发送凭据
});
Vue.use(ElementUI);
Vue.prototype.$echarts = echarts;
Vue.config.productionTip = false

new Vue({
  render: h => h(App),
  router
}).$mount('#app')
