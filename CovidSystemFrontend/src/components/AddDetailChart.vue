<template>
  <div id="charts"></div>
</template>

<script>
export default {
  name: "AddDetailChart",
  data(){
    return{
      historyDetails:{
        time:[],
        totalNow:[],
        totalConfirmAdd:[],
        totalHeal:[],
        totalDead:[],
        totalConfirm:[]
      }
    }
  },
  methods : {
    buildEchart(details){
      let myChart = this.$echarts.init(document.getElementById("charts"));
      let option = {
        tooltip: {
          trigger: 'axis'
        },
        legend: {
          data: ['新增确诊', '当前确诊', '累计治愈', '累计死亡','累计确诊']
        },
        grid: {
          left: '3%',
          right: '4%',
          bottom: '3%',
          containLabel: true
        },
        xAxis: {
          type: 'category',
          boundaryGap: false,
          data: details.time
        },
        yAxis: {
          type: 'value'
        },
        series: [
          {
            name: '新增确诊',
            type: 'line',
            data: details.totalConfirmAdd
          },
          {
            name: '当前确诊',
            type: 'line',
            data: details.totalNow
          },
          {
            name: '累计治愈',
            type: 'line',
            data: details.totalHeal
          },
          {
            name: '累计死亡',
            type: 'line',
            data: details.totalDead
          },
          {
            name: '累计确诊',
            type: 'line',
            data: details.totalConfirm
          }
        ]
      };
      myChart.setOption(option);
    }
  },
  mounted() {
    this.buildEchart(this.historyDetails);
  }
}
</script>

<style scoped>
#charts {
  width: 100%;
  height: 300px;
  border: none;
}
</style>