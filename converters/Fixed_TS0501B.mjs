import {identify, light} from 'zigbee-herdsman-converters/lib/modernExtend';

const definition = {
    zigbeeModel: ['Fixed_TS0501B'],
    model: 'Fixed_TS0501B',
    vendor: 'DocLab',
    description: 'Zigbee light',
    extend: [identify(), light({effect: false, ota: true})],
    meta: {},
};

export default definition;