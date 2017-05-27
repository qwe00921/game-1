===1.1.22.521===
1、添加 豪华曹操传 的游戏配置
2、修正 dic_gk 导出错误的问题
3、添加 精忠报国岳飞传 的游戏配置
===1.1.21.415===
1、将配置文件的字段命名改为与 json 名称一致
2、添加 imsg_hero_info_num ，用来配置读取角色信息的数量
===1.1.20.415===
1、添加 data.e5 读取 dic_hero.rimg_id 的字长判断，默认为读取 1 个字节
2、《宋江传》 data.e5 读取 dic_hero.rimg_id 的长度改为 2 个字节
===1.1.19.407===
1、添加 data.e5 读取 dic_hero.face_id 的字长判断，默认为读取 1 个字节
2、《宋江传》 data.e5 读取 dic_hero.face_id 的长度改为 2 个字节
===1.1.18.328===
1、恢复字段255判断（某些字段如果是255则直接变成0，否则变为原有值+1）
2、添加《宋江传》配置
3、添加《续三国志英杰传》配置
===1.1.17.306===
1、去除所有字段255判断（原有某些字段如果是255则直接变成0，否则变为原有值+1）
===1.1.16.212===
1、封神榜 dic_item.json 添加对 star.e5 的读取
===1.1.15.209===
1、修改 dic_hero.json 中的 "hid"，"face_id"，"rimg_id" 原来 256 为改为 0， 现在去掉特殊处理
===1.1.14.122===
1、修正曹操传配置读取错误的问题
===1.1.13.115===
1、dic_item.json 的 cid 输出值改为 0&
===1.1.12.115===
1、dic_hero.json 的 seid 添加 & 结尾,改为字符串类型
===1.1.11.115===
1、dic_item.json 的 seid 添加 & 结尾,改为字符串类型
===1.1.10.114===
1、excel 和 json 使用不同的目录
2、导出的 json 判断整数类型
3、json 导出添加缺少的字段
4、导出数据的时候同时导出 excel 和 json
5、导出为 excel 时增加数据类型判断
6、调整输出 json 字段的顺序
===1.1.9.113===
1、添加 Excel 和 JSON 转换的功能
===1.1.8.109===
1、修改 dic_item.json, dic_job.json, dic_skill.json 文件格式
===1.1.7.109===
1、添加游戏类型选择，加入东吴传配置
===1.1.6===
1、添加 dic_job_pk.json 生成
===1.1.5===
1、添加 dic_jobItem.json 生成
2、添加 dic_jobSkill.json 生成
===1.1.4===
1、添加 dic_jobTerrain.json 生成
2、添加 dic_jobWalk.json 生成
===1.1.3===
1、修改 dic_gk.json 中 gkid + 1
2、修改 dic_hero.json 的 info 字段
3、修改 dic_skill.json 的 info 字段
4、同样的程序已经上传到 ftp 服务器的 /APP/tools/ChessLmTool 目录
===1.1.2===
1、新版本完成了 dic_gk.json 的生成，注意文件中的数据都是原始数据，如需字段值 + 1，请产品给出相关文档
2、同样的程序已经上传到 ftp 服务器的 /APP/tools/ChessLmTool 目录
===1.1.1===
1、新版本完成了 Data.E5 的解析，需要验证处理结果。
2、新版本完成了 Imsg.e5 的解析。
3、同样的程序已经上传到 ftp 服务器的 /APP/tools/ChessLmTool 目录
===1.1.0===
1、原来版本可以解析 Chess.LM
2、新版本添加了 Data.E5 的解析，需要验证处理结果
3、Imsg.e5 的解析明天处理。
4、同样的程序已经上传到 ftp 服务器的 /APP/tools/ChessLmTool 目录