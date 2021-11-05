<template>
      <div id="main"></div>
</template>
<script>


export default {
  name: "ChinaMapLog",
  data() {
    return {
      select: false,
      dataList: [],
      mapData:[],
      completedCityName:[],
      selectedMap: 'china'
    }
  },
  methods: {
    /**
     * @function 构建地图，并在地图上显示数据
     * @data mapData 由{name : string,value : number}构成
     */
    buildMap() {
      let myChart = this.$echarts.init(document.getElementById('main'));
      let option = {
        visualMap: {
          min: 0,
          max: 400,
          left: 'left',
          top: 'bottom',
          text: ['高', '低'],//取值范围的文字
          inRange: {
            color: ['#fff4e6', '#dd2002']//取值范围的颜色
          },
          show: false//图注
        },
        geo: {
          map: this.selectedMap,
          roam: false,//不开启缩放和平移
          zoom: 1.25,//视角缩放比例
          label: {
            normal: {
              show: false,
              fontSize: '10',//注意：地图省份名字字体如果过大会导致字体重叠
              color: 'rgba(0,0,0,0.7)'
            }
          },
          itemStyle: {
            normal: {
              borderColor: 'rgba(0, 0, 0, 0.2)'
            },
            emphasis: {
              areaColor: '#F3B329',//鼠标选择区域颜色
              shadowOffsetX: 0,
              shadowOffsetY: 0,
              shadowBlur: 20,
              borderWidth: 0,
              shadowColor: 'rgba(0, 0, 0, 0.5)'
            }
          }
        },
        series: [
          {
            name: '现存确诊',
            type: 'map',
            geoIndex: 0,
            data: this.mapData
          }
        ]
      };
      myChart.setOption(option);
      myChart.off('dblclick');
      myChart.off('click');
      //控制跳转
      myChart.on('dblclick', (params) => {
        this.dblclickMap(params);
      });
      myChart.on('click', (params) => {
        this.clickMap(params);
      });
    },
    /**
     * @function: 获取当前选中地图的数据，填充至dataList与mapData中
     * @param: position (china|省份名)
     */
    async getDetailsOfPosition(position){
      let params = {}
      let isProvinceMap = position !== 'china';
      if(!isProvinceMap){
        params = {
          province: 'all',
          newest: true
        };
      }
      else{
        //填充补全数据到completedCityName中
        await this.importCompletedCity(position);
        params = {
          province: position,
          city: 'all',
          newest: true
        }
      }
      this.$axios.get('/details',{params:params}).then(
          (res) => {
            for(let item of res.data){
              //对省份地图的name进行补全
              if(isProvinceMap){
                for(let completedCity of this.completedCityName){
                  if(this.matchString(completedCity,item.City)){
                    item.City = completedCity;
                    break;
                  }
                }
              }
              let tmp = {
                value : item.TotalNow
              }
              tmp.name = isProvinceMap? item.City : item.Province;
              this.dataList.push(item);
              this.mapData.push(tmp);
            }
            this.buildMap();
            this.$parent.setDetailList(this.mapData);
            this.$parent.initButton();
          }
      );
    },
    //挂载时调用，初始化地图
   async initMap(){
     this.initChina();
     await this.getDetailsOfPosition('china');
    },
    //单击事件：切换显示数据
    clickMap(params){
      let ret = this.getDetailsByPosition(params.name);
      if(ret.isfind) {
        this.$parent.setDetails(ret);
        if(this.selectedMap === 'china')
          this.getHistoryDetails(params.name);
      }
      else{
        this.$message.error('地区未获取到相关数据！');
      }
    },
    /**
    *@function: 获取当前区域的疫情信息
    *@param: 地点
    *@return: details对应数据
    */
    getDetailsByPosition(position){
      let ret={
        Position : position,
        TotalNow : 0,
        TotalConfirmAdd : 0,
        TotalHeal : 0,
        TotalDead : 0,
        isfind : false
      };
      for (let item of this.dataList) {
        if (item.Province === position || item.City === position) {
          ret.TotalNow = item.TotalNow;
          ret.TotalHeal = item.TotalHeal;
          ret.TotalDead = item.TotalDead;
          ret.TotalConfirmAdd = item.TotalConfirmAdd;
          ret.isfind = true;
          break;
        }
      }
      return ret;
    },
    //切换地图
    setSelectedMap(province){
      console.log(province)
      if(province !== 'china') {
        require('echarts/map/js/province/' + this.provinceToJs(province));
        this.selectedMap = province;
      }
      else{
        this.selectedMap = 'china';
      }
    },
    /**
     * @function 负责导入补全市名至completedCityName
     * @param province:string  city:string
     */
    async importCompletedCity(province){
      this.completedCityName = [];
      // todo:填充省份地图
     await this.$axios('/area',{params:{province:'all'}}).then(
          (res)=>{
            for(let item of res.data){
              if(this.matchString(item,province)){
                province = item;
                break;
              }
            }
          }
      );
      let param = {
        province : province
      }
     if(province.charAt(province.length-1)==='市'){
       param = {
         city : province
       }
     }
      this.$axios('/area',{params:param}).then(
          (res)=>{
            for(let item of res.data){
                this.completedCityName.push(item);
              }
            }
      );
    },
    //负责由省份名到相关js的转换
    provinceToJs(province){
      let fileName = {
        '浙江': "zhejiang.js",
        '安徽': "anhui.js",
        '澳门': "aomen.js",
        '北京': "beijing.js",
        '重庆': "chongqing.js",
        '福建': "fujian.js",
        '甘肃': "gansu.js",
        '广东': "guangdong.js",
        '广西': "guangxi.js",
        '贵州': "guizhou.js",
        '海南': "hainan.js",
        '河北': "hebei.js",
        '黑龙江': "heilongjiang.js",
        '河南': "henan.js",
        '湖北': "hubei.js",
        '湖南': "hunan.js",
        '江苏': "jiangsu.js",
        '江西': "jiangxi.js",
        '吉林': "jilin.js",
        '辽宁': "liaoning.js",
        '内蒙古': "neimenggu.js",
        '宁夏': "ningxia.js",
        '青海': "qinghai.js",
        '山东': "shandong.js",
        '上海': "shanghai.js",
        '山西': "shanxi.js",
        '陕西': "shanxi1.js",
        '四川': "sichuan.js",
        '台湾': "taiwan.js",
        '天津': "tianjin.js",
        '香港': "xianggang.js",
        '新疆': "xinjiang.js",
        '西藏': "xizang.js",
        '云南': "yunnan.js"
      };
      return fileName[province];
    },
    //双击事件：切换地图
    dblclickMap(params){
      this.dataList = [];
      this.mapData = [];
      if(this.selectedMap === 'china') {
        if(params.name === '台湾' || params.name === '南海诸岛' || params.name === '香港'){
          this.$message({
            message: '地区未包含详细地图',
            type: 'error'
          });
          return;
        }
        this.setSelectedMap(params.name);
        this.getDetailsOfPosition(params.name);
      }
      else {
        this.setSelectedMap('china');
        this.getDetailsOfPosition('china');
        this.initChina();
      }
    },
    /**
     * @function 按次序模糊匹配字符串
     * @param template 规范名称
     * @param target 待匹配的字符串
     */
    matchString(template, target){
      if(template.length < target.length){
        return false;
      }
      let targetIndex = 0;
      let templateIndex = 0;
      while(templateIndex < template.length){
        if(template.charAt(templateIndex) === target.charAt(targetIndex)){
          targetIndex++;
        }
        if(targetIndex === target.length){
          return true;
        }
        templateIndex++;
      }
      return false;
    },
    /**
     * @function 获取历史信息，保留前五次
     * @param province : string 省份名
     * @param city : string 城市名
     * @return historyDetails
     */
    getHistoryDetails(province, city=''){
      let ret = {
        time:[],
        totalNow:[],
        totalConfirmAdd:[],
        totalHeal:[],
        totalDead:[],
        totalConfirm:[]
      };
      let timeSet = new Set();
      let repeatTimes = 0;
      let params = {
        province : province
      }
      if(city !== ''){
        params.city = city;
      }
      this.$axios.get('/details',{params : params}).then(
          (res)=>{
            res.data.reverse();
            for(let item of res.data){
              let time = item.UpdateTime.split('T')[0];
              if(timeSet.has(time)){
                continue;
              }
              else{
                timeSet.add(time);
              }
              ret.time.unshift(time);
              ret.totalNow.unshift(item.TotalNow);
              ret.totalConfirmAdd.unshift(item.TotalConfirmAdd);
              ret.totalHeal.unshift(item.TotalHeal);
              ret.totalDead.unshift(item.TotalDead);
              ret.totalConfirm.unshift(item.TotalNow+item.TotalHeal+item.TotalDead);
              repeatTimes++;
              if(repeatTimes === 5){
                this.$parent.setHistoryDetails(ret);
                break;
              }
            }
          }
      );
    },
    initChina(){
      let ret = {
        time:[],
        totalNow:[],
        totalConfirmAdd:[],
        totalHeal:[],
        totalDead:[],
        totalConfirm: []
      };
      let timeSet = new Set();
      let repeatTimes = 0;
      this.$axios.get('/histories').then(
          (res)=>{
            res.data.reverse();
            for(let item of res.data){
              let time = item.Ds.split('T')[0].split('-').join('/');
              if(timeSet.has(time)){
                continue;
              }
              else{
                timeSet.add(time);
              }
              ret.time.unshift(time);
              ret.totalNow.unshift(item.Confirm - item.Heal - item.Dead);
              ret.totalConfirmAdd.unshift(item.ConfirmAdd);
              ret.totalHeal.unshift(item.Heal);
              ret.totalDead.unshift(item.Dead);
              ret.totalConfirm.unshift(item.Confirm);
              repeatTimes++;
              if(repeatTimes === 5){
                this.$parent.setHistoryDetails(ret);
                break;
              }
            }
            let detail = {
              Position: "全国地区",
              TotalNow: ret.totalNow[ret.totalNow.length - 1],
              TotalDead: ret.totalDead[ret.totalDead.length - 1],
              TotalHeal: ret.totalHeal[ret.totalHeal.length - 1]
            }
            this.$parent.setDetails(
                detail
            );
          }
      );
    }
  },
  created() {
  },
  mounted() {
    this.initMap();
  }
}
</script>

<style scoped>

#main {
  width: 550px;
  height: 500px;
  border: none;
}
</style>