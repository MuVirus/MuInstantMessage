import grpc from "@grpc/grpc-js";
import message_proto from "./proto";
import { SendMail } from "./email";
import { email_user } from './config';
import { Errors } from './const'
import { GetRedis, SetRedisExpire } from './redis'

async function GetVarifyCode(call: any, callback: any) {
    // 获取email
    const email = call.request.email;
    console.log("目标邮箱: ", email);
    try {
        // 从Redis中获取token(前缀 + 目标邮箱)，如果获取到了，则表示验证码没有过期
        const query_res = await GetRedis(email);

        let code = query_res;

        // 如果redis中没有token信息，则添加kv数据，并设置过期时间
        if (query_res === null) {
            // 生成uuidv4, 作为验证码
            code = crypto.randomUUID();
            // 添加kv键值对，并添加过期时间
            await SetRedisExpire(email, code, 180);

            // 向目标邮箱发送验证码(不需要重复发验证码)
            // 发送文本
            const text_str = `您的验证码位${code}, 请在三分钟内完成注册`;
            const send_res = await SendMail({
                from: email_user,
                to: email,
                subject: "Chat验证码",
                text: text_str,
            });
        }

        // grpc回调
        callback(null, {
            email: email,
            error: Errors.Success,
            code: code,
        });
    } catch (error) {
        console.log("catch error is ", error);
        callback(null, {
            email: email,
            error: Errors.Exception,
        });
    }
}

function main(): void {
    const server = new grpc.Server();
    server.addService(message_proto.VarifyService.service, {
        GetVarifyCode: GetVarifyCode,
    });

    server.bindAsync(
        "0.0.0.0:50056",
        grpc.ServerCredentials.createInsecure(),
        () => {
            console.log("grpc server started on 0.0.0.0:50056");
        }
    )
}

main();