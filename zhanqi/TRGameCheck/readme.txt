===0.9.7.527===
1、buy_charge.good_chargeId 字段改为可选
2、去除 dic_use_effe.json 文件，修改 dic_item.ueid 检查范围为 0-21
===0.9.6.521===
1、忽略 dic_hero.name 字段重复检查
2、buy_charge.id 检查改为 buy_charge.good_chargeId 检查
===0.9.5.519===
1、取消 dic_seid 的 sename+special_effe 重复检查
2、添加单独检查 globalres 目录的功能
===0.9.4.515===
1、去除 dic_gk.gkname 检查；
2、guide.json 添加可选检查；
3、如果 globalres 目录存在，则检查 strings.xml 和 png, bmp 文件名小写；
4、删除 dic_job.imgid 对应的文件名检查；
5、dic_skill.skill_act 忽略 0
6、增加 buy_charge.json, good_charge.json, trpay.json 解析，并添加相关字段 0 和 1 范围解析
7、现在 曹魏传\正式发布资源 可以检查通过
===0.9.3.511===
1、dic_gk.buyid 忽略0的检查
2、忽略 img/meff 目录文件检查
3、dic_skill.skill_act 改为外键检查
4、dic_skill_act.imageid 改为 png 文件检查
===0.9.2.423===
1、修改检查文件扩展名小写
2、添加检查logo文件存在
3、存档文件应该不存在
4、添加资费相关的json文件检查存在
===0.9.1.422===
1、取消屏蔽的检查
2、dic_gk.buyid 添加最大值为 517 的判断
3、dic_gk.buyid>=500 时添加 -412 的处理
===0.8.1.417===
1、修改 xml 配置文件格式，添加 strings.xml 解析
2、注释掉部分检查指令
3、主界面日志功能添加清空按钮
===0.7.1.408===
1、检查 dic_jobSkill.skid 时忽略空字符串
2、dic_turn1 改为文件可选
3、map_terrain.effect 改为可以直接填 &
4、dic_seid改为 sename 和 special_effe 一起检查唯一
===0.6.1.402===
1、参考最新文档进行指令解析，添加了显示所有日志的开关
2、默认路径改为封神榜目录
3、添加对于 json 字段是否存在的检查
===0.2.1.331===
1、添加中文文件名检查，修改 xml 配置文件格式
2、修改出错信息为更容易识别
3、添加 string 类型的唯一判断，添加更多的 json 类型检查
4、添加 eex_new 剧本文件格式检查（注意当前只能检查格式，还无法检查字段值）
===0.0.1.328===
1、第一个版本，添加基本解析指令