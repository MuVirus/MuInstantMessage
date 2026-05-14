import nodemailer from 'nodemailer';
import {email_user, email_pass} from './config'

// Create a transporter using SMTP
const transporter = nodemailer.createTransport({
  host: "smtp.163.com",
  port: 465,
  secure: true, // use STARTTLS (upgrade connection to TLS after connecting)
  auth: {
    user: email_user,
    pass: email_pass,
  },
});

export function SendMail(mailOptions: nodemailer.SendMailOptions): Promise<string> {
    return new Promise((resolve, reject) => {
        transporter.sendMail(mailOptions, (error, info) => {
            if (error) {
                console.log(error);
                reject(error);
            } else {
                console.log("邮件已成功发送: " + info.response);
                resolve(info.response);
            }
        });
    });
}

// test
// await SendMail({
//     from: email_user,
//     to: "movmail@qq.com",
//     subject: "Test email",
//     text: "Hello world!",
// });