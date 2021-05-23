## MySQL8 源码安装

| 作者 | 时间 |QQ技术交流群 |
| ------ | ------ |------ |
| perrynzhou@gmail.com |2020/12/01 |672152841 |

### 系统安装包

```
// centos
yum -y install wget  cmake gcc gcc-c++ ncurses  ncurses-devel  libaio-devel  openssl openssl-devel cmake3 bison wget bzip2

// ubuntu debian
apt install build-essential cmake bison libncurses5-dev libssl-dev pkg-config
```

### MySQL8下载和编译

```
 // 下载boost
 wget https://boostorg.jfrog.io/artifactory/main/release/1.73.0/source/boost_1_73_0.tar.gz
 // /tmp目录是 -DWITH_BOOST 配置的目录
 mv boost_1_73_0.tar.gz /tmp/
 // 下载mysql8
 wget https://github.com/mysql/mysql-server/archive/refs/tags/mysql-8.0.23.tar.gz
 // 解压mysql-8.0.23.tar.gz
 tar zxvf mysql-8.0.23.tar.gz && cd mysql-server-mysql-8.0.23/
 cmake . \
-DDOWNLOAD_BOOST=1 \
-DWITH_BOOST=/tmp \
-DCMAKE_INSTALL_PREFIX=/usr/local/mysql \
-DMYSQL_DATADIR=/data1/mysql8/data \
-DWITHOUT_CSV_STORAGE_ENGINE=1 \
-DWITHOUT_FEDERATED_STORAGE_ENGINE=1 \
-DWITHOUT_ARCHIVE_STORAGE_ENGINE=1 \
-DFORCE_INSOURCE_BUILD=1 \
-DWITH_SSL=system \
-DWITH_DEBUG=1 
# -j 表示使用几个CPU同时编译
make -j 2
make install
```

### MySQL8 启动

- 配置文件

  ```
  [root@CentOS8 /data1]$ cat my.cnf 

[mysqld]
server-id=1
port=3306
datadir=/data1/mysql/data
log_error=/data1/mysql/log/error.log
pid_file=/data1/mysql/data/mysql.pid
  ```

- 初始化mysql
  ```
  mysqld --initialize-insecure --user=mysql --basedir=/usr/local/mysql --datadir=/data1/mysql8
  ```
- 启动mysql
  ```
  mysqld_safe --defaults-file=/data1/my.cnf

  ```

### 修改默认用户的密码

```
root@172.16.84.49 ~ $ mysql

mysql> alter user root@localhost identified by 'mysql';
Query OK, 0 rows affected (0.00 sec)

mysql> show engines;
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
| Engine             | Support | Comment                                                        | Transactions | XA   | Savepoints |
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
| BLACKHOLE          | YES     | /dev/null storage engine (anything you write to it disappears) | NO           | NO   | NO         |
| MRG_MYISAM         | YES     | Collection of identical MyISAM tables                          | NO           | NO   | NO         |
| MyISAM             | YES     | MyISAM storage engine                                          | NO           | NO   | NO         |
| PERFORMANCE_SCHEMA | YES     | Performance Schema                                             | NO           | NO   | NO         |
| InnoDB             | DEFAULT | Supports transactions, row-level locking, and foreign keys     | YES          | YES  | YES        |
| MEMORY             | YES     | Hash based, stored in memory, useful for temporary tables      | NO           | NO   | NO         |
| CSV                | YES     | CSV storage engine                                             | NO           | NO   | NO         |
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
7 rows in set (0.00 sec)

```

### 客户端模拟rollback

```
root@172.16.84.49 ~ $ mysql -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 11
Server version: 8.0.23-debug Source distribution
mysql> begin;
Query OK, 0 rows affected (0.00 sec)


mysql> create database test;
Query OK, 1 row affected (0.02 sec)

mysql> use test;
Database changed
mysql> create table test1(id int);
Query OK, 0 rows affected (0.06 sec)

mysql> commit;
Query OK, 0 rows affected (0.00 sec)

mysql> begin
    -> ^C
mysql> begin;
Query OK, 0 rows affected (0.00 sec)

mysql> insert into test1 select 1;
Query OK, 1 row affected (0.00 sec)
Records: 1  Duplicates: 0  Warnings: 0

mysql> rollback;
```
### 调试mysqld

```
root@172.16.84.49 ~ $ ps -ef|grep mysqld
root      45625  39470  0 16:12 pts/1    00:00:00 /bin/sh /usr/local/mysql/bin/mysqld_safe --defaults-file=/data1/my.cnf
mysql     45737  45625  0 16:12 pts/1    00:00:03 /usr/local/mysql/bin/mysqld --defaults-file=/data1/my.cnf --basedir=/usr/local/mysql --datadir=/data1/mysql8 --plugin-dir=/usr/local/mysql/lib/plugin --user=mysql --log-error=dgdpl1921.err --pid-file=dgdpl1921.pid --port=3306
root      48047  47974  0 16:21 pts/3    00:00:00 grep --color=auto mysqld
root@172.16.84.49 ~ $ gdb /usr/local/mysql/bin/mysqld
Reading symbols from /usr/local/mysql/bin/mysqld...
(gdb) br innobase_rollback
Breakpoint 1 at 0x47e8336: file /root/zhoulin/mysql-server-8.0.23/storage/innobase/handler/ha_innodb.cc, line 5481.
(gdb) attach 45737
Breakpoint 1 at 0x47e8336: file /root/zhoulin/mysql-server-8.0.23/storage/innobase/handler/ha_innodb.cc, line 5481.
(gdb) 
```
