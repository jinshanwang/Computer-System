# API测试程序说明文档

## 介绍

此文档主要详细说明测试程序中每个文件的主要职责。  
目前主要为以下**8**个文件，未来的测试场景如若有所增加，将继续优化测试脚本，并更新说明文档

 - senario01_simple_transaction.sh
 - wipe_Restart.sh
 - insert_Position.sh
 - init_database.sh
 - testPurchase.lua  
 - testSell.lua   
 - database_init.sql
 - database_position.sql


## senario01_simple_transaction.sh

此脚本为模拟测试场景一：简单的买卖操作。主要目的是在正式的测试场景触发前，对柜台服务的初始化完成作确认。确保后续的测试工作可以正常运行。
以下的代码将声明所需要的配置文件，在日后的工作中，如果需要改变买卖的细节，只需要更改配置文件信息

### 执行指令

```bash
./senario01_simple_transaction.sh
```

### 脚本详细信息

```bash
SIMPLE_PURCHASE="./testPurchase.lua"
SIMPLE_SELL="./testSell.lua"
```

重启柜台，每一个测试场景触发前都应该重启柜台服务并删除旧的数据信息，避免在测试中产生不必要的测试错误（eg. 队列size的问题）。启动完此脚本后，还需要等待一段时间，让柜台服务初始化正常。等待的时间应根据测试使用的机器性能作修改。

```bash
./wipeRestart.sh    #重新启动并删除队列以及持久化数据
sleep 20 
```

测试程序启动。XtCounterApiTest 为API测试程序，$SIMPLE_PURCHASE 为程序所需要的参数。
测试场景的不同，将会导致测试所需要的lua配置文件不同

```bash
../XtCounterApiTest $SIMPLE_PURCHASE
../XtCounterApiTest $SIMPLE_SELL
```


## wipe_Restart.sh

此脚本为主要作用为清理队列及持久化信息，并重新启动柜台。防止不合法的关闭柜台和启动柜台造成的 undefined behaviour。注意的是，此脚本不需要单独执行，可仅为所有测试场景的辅助脚本。

### 执行指令

```bash
./wipe_Restart.sh
```

## insert_Position.sh

此脚本作用为特定的测试场景插入持仓。

### 执行指令

```bash
./insert_Position.sh <以.sql为后缀的sql语句文件>
```

### 脚本详细信息

此信息可根据需要修改

```bash
USER="root"           # 用户名
DATABASE="counter"    # 数据库名
PASSWORD="mysql.rzrk" # 密码
SQLFILE=$1            # 脚本命令参数作为数据库输入的文件
```

以下命令将会执行数据插入操作

```bash
mysql -u$USER -p$PASSWORD << $SQLFILE 2>/dev/null 
[ $? -eq 0 ] && echo "插入成功"
```

## init_database.sh

### 执行指令

```bash
./init_database.sh ./database_init.sql
```

### 脚本详细信息

脚本内容与 insert_Position.sh类似。

## testPurchase.lua / testSell.lua

### 脚本详细信息

此类文件将作为XtCounterApiTest 的参数，XtCounterApiTest 将读取此文件中的买卖操作时的主要交易信息。本测试脚本支持多账户交易，所以每一份报单的数据以" , "分隔。其中包括以下几点：

  1. 报单次数

  ```lua
  --修改报单次数
function getOrderTimes()
    return "2"
end
  ```

  2. 报单频率（ms）

```lua
  --报单次数5ms
function getOrderInterval()
    return "1000"
end
```

  3. 报单股票代码

 ```lua
  --股票代码
function getOrderCode()
    return "603077,603077"
end
 ```

  4. 报单价格

```lua
function getOrderPrice()
    return "152,152" --1.52元
end
```

  5. 报单数量

  ```lua
 --报单数量
function getOrderVolume()
    return "100,100"  -- 100股
end
  ```

  6. 报单市场编号（上海/深圳）

 ```lua
 --市场代号 1：上海 2：深圳
function getMarket()
    return "1,1"
end
 ```

  7. 买卖类型

 ```lua
 function getOperationType()
	return "0,0"
end
 ```

  9. 账号信息
     以下为账户的所需要的登录以及下单的个人信息 

 ```lua
function getAccountId()     -- 账户ID
    return "1000000003,1000000004"
end
function getAccountPass()   -- 账户密码
    return "123456,123456"
end
function getShSecuAccount() -- 上海市场的股东号
    return "A279102097,SH60000004"
end
function getSzSecuAccount() -- 深圳市场的股东号
    return "0128924290,SZ60000004"
end
 ```

后续的测试工作中，测试人员可以根据测试场景需要重新编写配置配置文件并修改以上所有的**lua**函数。

## 测试流程图



![流程图](C:\Users\a1710974\Desktop\simpleTestCase\测试流程图.png)

