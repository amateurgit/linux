#!/bin/bash

cat << EOF

请按照以下步骤, 依次执行测试命令

    Step 0 : 配置 Web 登录账户, 以及邮件发送服务器信息

         vim webapp/.config

    Step 1 : 创建数据库表

        ./twofactor.sh --db_two_factor     # 创建双因子相关的数据库表
    
    Step 2 : 管理员 admin 登录
    
        ./twofactor.sh --admin-login
    
    Step 3 : 配置邮件服务器和发件人信息

        ./twofactor.sh --config-mailserver
    
    Step 4 : 创建 test 账户, 并配置邮箱
    
        ./twofactor.sh --add_user_test   
    
    Step 5 : 发送验证码

        ./twofactor.sh --send_captcha
    
    Step 6 : 登录测试(查看邮箱, 填写验证码)

        ./twofactor.sh --test-login-with-captcha=xxxxxx
EOF
