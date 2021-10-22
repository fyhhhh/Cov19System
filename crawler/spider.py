import sys
import time
import pymysql
import json
import traceback
import requests
from selenium.webdriver import Chrome, ChromeOptions

province_list = ['北京', '天津', '河北', '山西', '内蒙古', '辽宁', '吉林', '黑龙江', '上海', '江苏', '浙江', '安徽', '福建', '江西', '山东',
                 '河南', '湖北', '湖南', '广东', '广西', '海南', '重庆', '四川', '贵州', '云南', '西藏', '陕西', '甘肃', '青海', '宁夏',
                 '新疆']
headers = {
    "user-agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36"
}


def get_province_data():
    conn = None
    cursor = None
    conn, cursor = get_conn()
    try:
        sql_test = "truncate hospitals"
        print('清空旧信息')
        cursor.execute(sql_test)
        conn.commit()
    except:
        print('error')

    for province in province_list:
        page_index = 1
        flag = 1
        while flag:
            url = "https://apis.map.qq.com/place_cloud/search/region?region=" + province \
                  + "&table_id=0ohEwy0ongOP8nIPH1&orderby=id&page_index=" + str(page_index) \
                  + "&page_size=200&key=YNVBZ-FRJK3-BPX36-3XHBZ-U7WFQ-KBFMJ"
            r = requests.get(url, headers)
            res = json.loads(r.text)
            result = res["result"]
            if result["data"] != []:
                data = result["data"]
                for detail in data:
                    Province = detail["province"]
                    City = detail["city"]
                    District = detail["district"]
                    Address = detail["address"]
                    Title = detail["title"]
                    Tel = detail["tel"].replace('\r\n', ' ')
                    #Location = str(detail["location"]['lat']) + " " + str(detail["location"]['lng'])
                    lat = str(detail["location"]["lat"])
                    lng = str(detail["location"]["lng"])
                    print(Province, City, District, Address, Title, Tel, lat, lng)
                    # 插入数据
                    try:
                        sql = "insert into hospitals(province, city, district, address, title, tel, lat, lng) values('%s','%s','%s','%s','%s','%s','%s', '%s')" % (
                        Province, City, District, Address, Title, Tel, lat, lng)
                        print(f"{time.asctime()}开始更新数据")
                        cursor.execute(sql)
                        conn.commit()
                        print(f"{time.asctime()}已是最新数据！")

                    except:
                        traceback.print_exc()

                page_index += 1
            else:
                flag = 0

    close_conn(conn, cursor)

def get_risk_info():
    url = "https://eyesight.news.qq.com/sars/riskarea"
    conn = None
    cursor = None
    conn, cursor = get_conn()
    try:
        sql_test = "truncate risk_areas"
        print('清空旧信息')
        cursor.execute(sql_test)
        conn.commit()
    except:
        print('error')

    r = requests.get(url, headers)
    res = json.loads(r.text)
    data = res['data']
    if data != []:
        for info in data:
            print(info['area'], info['type'])
            print('名称长度：',len(info['area']))
            try:
                sql = "insert into risk_areas(risk_level, address) values('%s','%s')" % (str(info['type']), info['area'])
                print(f"{time.asctime()}开始更新数据")
                cursor.execute(sql)
                conn.commit()
                print(f"{time.asctime()}已是最新数据！")

            except:
                traceback.print_exc()
    else:
        print("全国不存在中高风险地区！")

def get_tencent_data():
    url1 = "https://view.inews.qq.com/g2/getOnsInfo?name=disease_h5"
    url2 = "https://view.inews.qq.com/g2/getOnsInfo?name=disease_other"
    r1 = requests.get(url1, headers)
    r2 = requests.get(url2, headers)

    res1 = json.loads(r1.text)
    res2 = json.loads(r2.text)

    data_all1 = json.loads(res1["data"])
    data_all2 = json.loads(res2["data"])

    history = {}
    for i in data_all2["chinaDayList"]:
        ds = "2021." + i["date"]
        tup = time.strptime(ds, "%Y.%m.%d")  # 匹配时间
        ds = time.strftime("%Y-%m-%d", tup)  # 改变时间格式
        confirm = i["confirm"]
        suspect = i["suspect"]
        heal = i["heal"]
        dead = i["dead"]
        history[ds] = {"confirm": confirm, "suspect": suspect, "heal": heal, "dead": dead}
    for i in data_all2["chinaDayAddList"]:
        ds = "2021." + i["date"]
        tup = time.strptime(ds, "%Y.%m.%d")  # 匹配时间
        ds = time.strftime("%Y-%m-%d", tup)  # 改变时间格式
        confirm = i["confirm"]
        suspect = i["suspect"]
        heal = i["heal"]
        dead = i["dead"]
        history[ds].update({"confirm_add": confirm, "suspect_add": suspect, "heal_add": heal, "dead_add": dead})

    details = []
    update_time = data_all1["lastUpdateTime"]
    data_country = data_all1["areaTree"]
    data_province = data_country[0]["children"]
    for pro_infos in data_province:
        province = pro_infos["name"]
        for city_infos in pro_infos["children"]:
            city = city_infos["name"]
            confirm = city_infos["total"]["confirm"]
            confirm_add = city_infos["today"]["confirm"]
            heal = city_infos["total"]["heal"]
            dead = city_infos["total"]["dead"]
            details.append([update_time, province, city, confirm, confirm_add, heal, dead])
    return history, details


def get_conn():
    # 建立连接
    conn = pymysql.connect(host="localhost", user="root", password="zxy0517", db="Cov19", charset="utf8")
    # c创建游标
    cursor = conn.cursor()
    return conn, cursor


def close_conn(conn, cursor):
    if cursor:
        cursor.close()
    if conn:
        conn.close()


# 定义更新细节函数
def update_details():
    cursor = None
    conn = None
    try:
        li = get_tencent_data()[1]  # 1代表最新数据
        conn, cursor = get_conn()
        sql = "insert into details(update_time,province,city,confirm,confirm_add,heal,dead) values(%s,%s,%s,%s,%s,%s,%s)"
        sql_query = 'select %s=(select update_time from details order by id desc limit 1)'
        # 对比当前最大时间戳
        cursor.execute(sql_query, li[0][0])
        if not cursor.fetchone()[0]:
            print(f"{time.asctime()}开始更新数据")
            for item in li:
                cursor.execute(sql, item)
            conn.commit()
            print(f"{time.asctime()}更新到最新数据")
        else:
            print(f"{time.asctime()}已是最新数据！")
    except:
        traceback.print_exc()
    finally:
        close_conn(conn, cursor)


# 插入历史数据
def insert_history():
    cursor = None
    conn = None
    try:
        dic = get_tencent_data()[0]  # 0代表历史数据字典
        print(f"{time.asctime()}开始插入历史数据")
        conn, cursor = get_conn()
        sql = "insert into historys values (%s,%s,%s,%s,%s,%s,%s,%s,%s)"
        for k, v in dic.items():
            cursor.execute(sql, [k, v.get("confirm"), v.get("confirm_add"), v.get("suspect"),
                                 v.get("suspect_add"), v.get("heal"), v.get("heal_add"),
                                 v.get("dead"), v.get("dead_add")])
        conn.commit()
        print(f"{time.asctime()}插入历史数据完毕")
    except:
        traceback.print_exc()
    finally:
        close_conn(conn, cursor)


# 更新历史数据
def update_history():
    cursor = None
    conn = None
    try:
        dic = get_tencent_data()[0]  # 0代表历史数据字典
        print(f"{time.asctime()}开始更新历史数据")
        conn, cursor = get_conn()
        sql = "insert into historys values (%s,%s,%s,%s,%s,%s,%s,%s,%s)"
        sql_query = "select confirm from historys where ds=%s"
        for k, v in dic.items():
            if not cursor.execute(sql_query, k):
                cursor.execute(sql, [k, v.get("confirm"), v.get("confirm_add"), v.get("suspect"),
                                     v.get("suspect_add"), v.get("heal"), v.get("heal_add"),
                                     v.get("dead"), v.get("dead_add")])
        conn.commit()
        print(f"{time.asctime()}历史数据更新完毕")
    except:
        traceback.print_exc()
    finally:
        close_conn(conn, cursor)


if __name__ == "__main__":
    print("第一步：更新全国疫情历史数据")
    update_history()
    print("第二步：更新各地目前疫情数据")
    update_details()
    print("第三步：更新全国新冠检测点数据")
    choose = input("由于数据变化不大，本操作不需要频繁执行，是否继续？(N/y)：")
    if choose == 'y':
        get_province_data()
    else:
        print('跳过')
        pass
    print("第四步：更新中高风险地区信息 结果中 1 表示中风险，2 表示高风险") ## 网址变更？
    get_risk_info()
