<template>
  <div id="mapDetail" >
  </div>
</template>

<script>
export default {
  /**
   * @component 生成横向柱状图
   * @prop detailList {name : string,value : string}
   */
  name: "MapDetail",
  data(){
    return{
      valueSet:[],
      keySet:[],
      height: '400px'
    }
  },
  methods:{
    initData(detailList){
      this.valueSet = [];
      this.keySet = [];
      if(!detailList.empty)
        detailList.sort((a,b)=>{return a.value-b.value});
      for(let item of detailList){
        if(item.value === 0)
            continue;
        this.valueSet.push(item.value);
        this.keySet.push(item.name);
      }
    },
    buildMapDetail(detailList){
      this.initData(detailList);
      console.log(this.keySet)
      this.height = (this.valueSet.length *15+20)+'px';
      let myChart = this.$echarts.init(document.getElementById("mapDetail"));
      let option = {
        legend: {
        },
        grid: {
          left: '3%',
          right: '4%',
          bottom: '3%',
          top:'0%',
          height: this.height,
          containLabel: true
        },
        xAxis: {
          type: 'value',
          boundaryGap: [0, 0.01],
          show : false
        },
        yAxis: {
          type: 'category',
          offset: 0,
          data: this.keySet,
          axisLine: {
            show: false
          },
          axisTick:{
            show:false//不显示网格线
          },
          axisLabel:{
            textStyle: {
              fontSize: '10',
              align: 'right'
            }
          }
        },          offset : 0,
        series: [
          {
            type: 'bar',
            data: this.valueSet,
            label: {
              normal: {
                show: true,
                position: 'right',
                textStyle: {
                  color: 'black',
                  fontSize: 8
                }
              }
            },
            color: 'blue'
          }
        ]
      };
      myChart.setOption(option);
    }
  }
}
</script>

<style scoped>
#mapDetail{
  width: 300px;
  height: 500px;
  border: none
}
</style>