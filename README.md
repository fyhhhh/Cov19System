# **常态化疫情分析平台**

## 更新日志

##### 2021.10.20

---update---

1. 优化了后端数据计算，前端不再需要计算确诊数

2. 增加了省级查询，可以查询省级信息



##### 2021.10.22 16:00

---update---

1. 更新了GET Details接口，现在可通过all批量查询省或市级信息
2. 更新了GET details和爬虫部分的文档
3. 通过exe同目录下新建passwd.txt文件存储数据库密码便于调试

---fixed---

1. 修复了地区未确定可能出现的负数现存确诊问题



##### 2021.10.22 16:30

---update---

1. 整理了其它项目文件
2. 所有项目代码被整合至此仓库，不再只是后端

## **1 整体设计**

本项目旨在实现一个常态化疫情分析平台，将包含以下几个模块：

1. 基于Python语言实现的数据获取爬虫模块
2. 基于MySQL数据库的数据存储模块
3. 基于Go语言实现的Web服务端及数据库交互模块
4. 基于JavaScript实现的Web网页客户端及用户交互模块
5. 基于C语言实现的Linux嵌入式终端模块
6. 基于C++语言实现的Windows-PC可控管理端模块

## **2 基于Python语言实现的数据获取爬虫模块**

使用Python爬虫从网站获取信息

目标网站为 https://news.qq.com/zt2020/page/feiyan.htm#/?pool=heilongjiang&ADTAG=hesuan

### 2.1 更新全国疫情历史数据

**url：**"https://view.inews.qq.com/g2/getOnsInfo?name=disease_h5"

**数据结构：**
时间、累计确诊数、当日新增确诊、累计疑似确诊数、当日新增确诊数、累积治愈数、当日新增治愈、累计死亡数、当日新增死亡数

### 2.2 更新当前全国各地的疫情数据

**url：**"https://view.inews.qq.com/g2/getOnsInfo?name=disease_other"

**数据结构：**
id、时间、省份、城市、累计确诊数、当日新增确诊、累计治愈数、累计死亡数

### 2.3 更新全国新冠检测点数据（本步骤可选）

**url格式：**

`https://apis.map.qq.com/place_cloud/search`

`/region?region=[省/直辖市]&table_id=0ohEwy0ongOP8nIPH1&orderby=id`

`&page_index=[结果页数]&page_size=[页面记录条数(100/200)]&key=YNVBZ-FRJK3-BPX36-3XHBZ-U7WFQ-KBFMJ`

**数据结构：**
id、省份、城市、地区、详细地址、检测站名称、联系电话、经度、纬度

### 2.4 更新当前全国中高风险地区信息

**url：**"https://eyesight.news.qq.com/sars/riskarea"

**数据结构：**
id、风险级别、风险地区地址

## **3 基于MySQL数据库的数据存储模块**

使用*MySQL*数据库存储获取到的信息

建立 *tables* 如下：

1. *historys* 存储每日新增和累计的确诊、疑似、治愈、死亡数据
2. *details* 存储每日新确诊病例的地区及该地区累计和新增的确诊人数
3. *hospitals* 存储定点核酸检测医院的省市县区及地址、名称、电话和经纬度
4. *risk_areas* 存储目前中高风险地区地址及等级
5. *reports* 存储由嵌入式设备汇报管理端审核的突发警报信息

## 4. 基于Go语言实现的Web服务端及数据库交互模块

以Go语言为基础，基于Gin框架和GORM工具的MySQL数据库操作进程

本模块基于项目需求提供如下接口供其他模块调用：

### 4.1 提供给用户客户端的接口

#### 4.1.1 区县级的疫情现状查询

##### 4.1.1.1 获取全部

GET [IP]:[PORT]/details

```json
RETURN JSON:
[
    {
        "Id": 2,
        "UpdateTime": "2021-09-24T14:07:56+08:00",
        "Province": "福建",
        "City": "厦门",
        "Confirm": 261,
        "ConfirmAdd": 11,
        "Heal": 39,
        "Dead": 0
    },
]
```

|     Key      |    Value     |
| :----------: | :----------: |
|  *Confirm*   | 历史累计确诊 |
| *ConfirmAdd* | 当日新增确诊 |
|    *Heal*    | 历史累计治愈 |
|     Dead     | 历史累计死亡 |

***tips*：现存确诊 = *Confirm - Heal - Dead***

##### 4.1.1.2 指定查询

GET [IP]:[PORT]/details?province=[省/直辖市] (&city=[市/直辖市区]) (&newest=true)

本节所用字段键值含义如下：

|        Key        |    Value     |
| :---------------: | :----------: |
|    *TotalNow*     |  现存确诊数  |
| *TotalConfirmAdd* | 当日新增确诊 |
|    *TotalHeal*    | 历史累计治愈 |
|    *TotalDead*    | 历史累计死亡 |

①查询某省历史数据

```json
GET localhost:8080/details?province=福建
RETURN JSON:
[
    {
        "UpdateTime": "2021-09-24T14:07:56+08:00",
        "Province": "福建",
        "TotalNow": 474,
        "TotalConfirmAdd": 16,
        "TotalHeal": 789,
        "TotalDead": 1
    },
]
```

②查询某省最新一条数据

```Json
GET localhost:8080/details?province=福建&newest=true
RETURN JSON:
[
    {
        "UpdateTime": "2021-10-15T13:56:48+08:00",
        "Province": "福建",
        "TotalNow": 81,
        "TotalConfirmAdd": 0,
        "TotalHeal": 1212,
        "TotalDead": 1
    }
]
```

③查询某市历史数据

```json
GET localhost:8080/details?province=福建&city=厦门
RETURN JSON:
[
    {
        "UpdateTime": "2021-09-24T14:07:56+08:00",
        "Province": "福建",
        "City": "厦门",
        "TotalNow": 222,
        "TotalConfirmAdd": 11,
        "TotalHeal": 39,
        "TotalDead": 0
    },
]
```

④查询某市最新一条数据

```json
GET localhost:8080/details?province=福建&city=厦门&newest=true
RETURN JSON:
[
    {
        "UpdateTime": "2021-10-15T13:56:48+08:00",
        "Province": "福建",
        "City": "厦门",
        "TotalNow": 74,
        "TotalConfirmAdd": 0,
        "TotalHeal": 201,
        "TotalDead": 0
    }
]
```

##### 4.1.1.3 查询所有省级数据

GET [IP]:[PORT]/details?province=all (&newest=true)

```json
GET localhost:8080/details?province=all&newest=true
RETURN JSON:
[
    {
        "UpdateTime": "2021-10-15T13:56:48+08:00",
        "Province": "台湾",
        "TotalNow": 1593,
        "TotalConfirmAdd": 17,
        "TotalHeal": 13742,
        "TotalDead": 841
    },
]
```

##### 4.1.1.4 查询某省/直辖市下所有市/直辖市区级数据

GET [IP]:[PORT]/details?province=[省/直辖市] &city=all (&newest=true)

```json
GET localhost:8080/details?province=福建&city=all&newest=true
RETURN JSON:
[
    {
        "UpdateTime": "2021-10-15T13:56:48+08:00",
        "Province": "福建",
        "City": "厦门",
        "TotalNow": 222,
        "TotalConfirmAdd": 11,
        "TotalHeal": 39,
        "TotalDead": 0
    },
]
```



#### 4.1.2 疫情历史数据查询

GET [IP]:[PORT]/histories

```json
RETURN JSON:
[
    {
        "Ds": "2021-08-16T00:00:00+08:00",
        "Confirm": 122433,
        "ConfirmAdd": 55,
        "Suspect": 1,
        "SuspectAdd": 1,
        "Heal": 112893,
        "HealAdd": 60,
        "Dead": 5669,
        "DeadAdd": 0
    },
]
```

#### 4.1.3 中高风险区查询

GET [IP]:[PORT]/risk_areas

```jso
RETURN JSON:
[
    {
        "Id": 2,
        "RiskLevel": "1",
        "Address": "福建省莆田市仙游县郊尾镇后沈村"
    },
]
```

#### 4.1.4 核酸检测地点查询

GET [IP]:[PORT]/hospitals?province=[省/直辖市]&city=[市/直辖市区]

```json
GET localhost:8080/hospitals?province=陕西省&city=西安市
RETURN JSON:
[
    {
        "Id": 7148,
        "Province": "陕西省",
        "City": "西安市",
        "District": "碑林区",
        "Address": "陕西省西安市雁塔区翠华路345号",
        "Title": "西安市雁塔区疾病预防控制中心",
        "Tel": "0298-5263293",
        "Lat": 34.231994,
        "Lng": 108.96329
    },
]
```

##### 4.1.5 突发疫情信息查询

GET [IP]:[PORT]/reports?province=[省/直辖市]&city=[市/直辖市区]

```json
GET localhost:8080/reports?province=陕西省&city=西安市
RETURN JSON:
[
    {
        "Id": 1,
        "Province": "陕西省",
        "City": "西安市",
        "District": "西工大长安校区",
        "Info": "检测到体温异常"
    },
]
```



### 4.2 提供给管理客户端的接口

##### 4.2.1 突发疫情信息汇报

POST [IP]:[PORT]/reports

```json
POST JSON:
{
    "Province": "陕西省",
    "City": "西安市",
    "District": "西工大长安校区",
    "Info": "检测到体温异常"
}
```

##### 4.2.2 突发疫情解除

POST  [IP]:[PORT]/reports_delete

```json
POST JSON:
{
    "Province": "陕西省",
    "City": "西安市",
    "District": "西工大长安校区",
    "Info": "检测到体温异常"
}
```



## 5 基于JavaScript实现的Web网页客户端及用户交互模块

（待完善）



## 6 基于C语言实现的Linux嵌入式终端模块

（待完善）



## 7 基于C++语言实现的Windows-PC可控管理端模块

（待完善）