create database if not exists test character set utf8;

use test;

create table if not exists student (
    id int(11) not null auto_increment primary key,
    name varchar(20) not null default '' comment '姓名',
    age date comment '出生年月',
    sex enum('男','女') not null default '男' comment '性别'
) engine=innoDB default charset=utf8 comment '学生表';

create table if not exists teacher (
    id int(11) not null auto_increment primary key,
    name varchar(20) not null default '' comment '姓名'
) engine=innoDB default charset=utf8 comment '教师表';

create table if not exists course (
    id int(11) not null auto_increment primary key,
    name varchar(20) not null default '' unique comment '课程名',
    tid int(11) not null default 0 comment '教师id',
    foreign key(tid) references teacher(id)			-- 外键
) engine=innodb default charset=utf8 comment '课程表';


create table if not exists score (
    sid int(11) not null default 0 comment '学生ID',
    cid int(11) not null default 0 comment '课程编号',
    score int(11) not null default 0 comment '分数'
) engine=innodb default charset=utf8 comment '成绩表';

-- student 插入数据
insert into student values
 (1,'赵雷','1990-01-01','男'),
 (2,'钱电','1990-12-21','男'),
 (3,'孙风','1990-05-20','男'),
 (4,'李云','1990-08-06','男'),
 (5,'周梅','1991-12-01','女'),
 (6,'吴兰','1992-03-01','女'),
 (7,'郑竹','1989-07-01','女'),
 (8,'王菊','1990-01-20','女');


-- teacher 插入数据 注意 : teacher 表先插入数据，才可以初始化课程表
insert into teacher values
 (1,'张老师'),
 (2,'李老师'),
 (3,'王老师');


-- course 插入数据
insert into course values
 (1,'语文', 1),
 (2,'数学', 2),
 (3,'英语', 3);

-- score 插入数据
insert into score values
 (1,1,90),
 (1,2,80),
 (1,3,90),
 (2,1,70),
 (2,2,60),
 (2,3,80),
 (3,1,80),
 (3,2,80),
 (3,3,80),
 (4,1,50),
 (4,2,30),
 (4,3,20),
 (5,1,76),
 (5,2,87),
 (6,1,31),
 (6,3,34),
 (7,2,89),
 (7,3,98);
