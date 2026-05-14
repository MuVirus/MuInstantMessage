import path from 'path'
import * as grpc from "@grpc/grpc-js"
import * as protoLoader from  "@grpc/proto-loader"

const PROTO_PATH = path.join(import.meta.dir, 'message.proto');

const packageDefinition = protoLoader.loadSync(PROTO_PATH,     
    {keepCase: true,
     longs: String,
     enums: String,
     defaults: true,
     oneofs: true
    });

const protoDescriptor = grpc.loadPackageDefinition(packageDefinition);
const message_proto = protoDescriptor.message as any;

export default message_proto;
