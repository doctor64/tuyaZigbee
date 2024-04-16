const fz = require('zigbee-herdsman-converters/converters/fromZigbee');
const tz = require('zigbee-herdsman-converters/converters/toZigbee');
const exposes = require('zigbee-herdsman-converters/lib/exposes');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
const extend = require('zigbee-herdsman-converters/lib/extend');
const ota = require('zigbee-herdsman-converters/lib/ota');
const tuya = require('zigbee-herdsman-converters/lib/tuya');
const utils = require('zigbee-herdsman-converters/lib/utils');
const globalStore = require('zigbee-herdsman-converters/lib/store');
const e = exposes.presets;
const ea = exposes.access;

const DataType = {
    uint8: 0x20,
    uint16: 0x21,
    uint24: 0x22,
    uint32: 0x23,
    enum8: 0x30,
};
const manufacturerOptions = {
    doclab : {manufacturerCode: 0x6464}
};
const zcl_attr_batt_check_rate = 0x6400;

const fz_Poll_control = {
    cluster: 'genPollCtrl',
    type: ['attributeReport', 'readResponse'],
    convert: (model, msg, publish, options, meta) => {
        const result = {};

        if(msg.data.hasOwnProperty('checkinInterval')) {
            result['checkin_rate'] = msg.data['checkinInterval'];
        }
        if(msg.data.hasOwnProperty('longPollInterval')) {
            result['long_poll_rate'] = msg.data['longPollInterval'];
        }        
        if(msg.data.hasOwnProperty('shortPollInterval')) {
            result['short_poll_rate'] = msg.data['shortPollInterval'];
        }
        if(msg.data.hasOwnProperty('fastPollTimeout')) {
            result['fast_poll_timeout'] = msg.data['fastPollTimeout'];
        }        
        if(msg.data.hasOwnProperty(zcl_attr_batt_check_rate)) {
            result['battery_check_rate'] = msg.data[zcl_attr_batt_check_rate];
        }
        return result;
       }
}
const tz_Poll_Control = {
    key: ['checkin_rate', 'long_poll_rate', 'short_poll_rate', 'fast_poll_timeout', 'battery_check_rate'],
    convertGet: async (entity, key, meta) => {
        meta.logger.debug(`key ${key}`);
        switch(key) {
        case 'checkin_rate':
            entity.read('genPollCtrl',['checkinInterval']);
            break;
        case 'long_poll_rate':
            entity.read('genPollCtrl',['longPollInterval']);
            break;
        case 'short_poll_rate':
            entity.read('genPollCtrl',['shortPollInterval']);
            break;
        case 'fast_poll_timeout':
            entity.read('genPollCtrl',['fastPollTimeout']);
            break;
        case 'battery_check_rate':
            entity.read('genPollCtrl',[zcl_attr_batt_check_rate]);
            break;
        }

   },
    convertSet: async (entity, key, value, meta) => {
        let payload = {};
        let newValue = value;

        switch(key) {
            case 'checkin_rate':
                await entity.write('genPollCtrl', {'checkinInterval': (value)});
                break;
            case 'long_poll_rate':
                await entity.command('genPollCtrl', 'setLongPollInterval', {'newLongPollInterval': (value)}); 
                break;
            case 'short_poll_rate':
                await entity.command('genPollCtrl', 'setShortPollInterval', {'newShortPollInterval': (value)}); 
                break;
            case 'fast_poll_timeout':
                await entity.write('genPollCtrl', {'fastPollTimeout': (value)});
                break;                
            case 'battery_check_rate':
                payload = {0x6400: {'value': value, 'type': DataType.uint32}};
                await entity.write('genPollCtrl', payload);//, manufacturerOptions.doclab);
                break;
        }
    }
}


const definition = {
    zigbeeModel: ['Fixed_ZBWS01A_TS0041'], // The model ID from: Device with modelID 'lumi.sens' is not supported.
    model: 'Fixed ZBWS01A TS0041', // Vendor model number, look on the device for a model number
    vendor: 'DocLab, ex TuYa', // Vendor of the device (only used for documentation and startup logging)        
    description: 'Wireless switch with 1 button', // Description of the device, copy from vendor site. (only used for documentation and startup logging)
    fromZigbee: [fz.command_toggle, fz.battery, fz_Poll_control],
    toZigbee: [tz.factory_reset, tz_Poll_Control],
        configure: async (device, coordinatorEndpoint, logger) => {
            //binds to coordinator
            const firstEndpoint = device.getEndpoint(1);
            await reporting.bind(firstEndpoint, coordinatorEndpoint, ['genOnOff', 'genLevelCtrl', 'genPowerCfg']);

            // configure default poll control
            const chkInInterval = 3600; // 1 hour is default check-in interval
            await firstEndpoint.write('genPollCtrl', {'checkinInterval': (chkInInterval * 4)});
            const longPollInterval = 5; // 5 seconds is default timeout
            await firstEndpoint.command('genPollCtrl', 'setLongPollInterval', {'newLongPollInterval': (longPollInterval * 4)}); 

            // configure default battery check rate
            const batt_check_rate = 5000; // 5 sec default battery check rate
            const payload = {0x6400: {'value': batt_check_rate, 'type': DataType.uint32}};
            await firstEndpoint.write('genPollCtrl', payload);//, manufacturerOptions.doclab);

            // configure default reporting
            const overides = {min: 0, max: 3600, change: 0};
            await reporting.batteryVoltage(firstEndpoint, overides);
            await reporting.batteryPercentageRemaining(firstEndpoint, overides);

            //get current status
            await firstEndpoint.read('genPollCtrl', ['checkinInterval']);
            await firstEndpoint.read('genPollCtrl', ['longPollInterval']);
            await firstEndpoint.read('genPollCtrl', ['shortPollInterval']);
            await firstEndpoint.read('genPollCtrl', ['fastPollTimeout']);
            await firstEndpoint.read('genPollCtrl', [zcl_attr_batt_check_rate]);
        },
    icon: ' data:image/png;base64,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',
    exposes: [
        e.battery(), e.battery_voltage(), e.action(['toggle','on', 'off', 'brightness_move_up', 'brightness_stop']), 
        exposes.numeric('checkin_rate', ea.ALL).withDescription('Check In, delay between uplink polls, in 1/4 s'),
        exposes.numeric('long_poll_rate', ea.ALL).withDescription('Long poll rate in 1/4 s'), 
        exposes.numeric('short_poll_rate', ea.ALL).withDescription('Fast poll rate in 1/4 s'),
        exposes.numeric('fast_poll_timeout', ea.ALL).withDescription('Fast poll timeout in 1/4 s'),
        exposes.numeric('battery_check_rate', ea.ALL).withDescription('Delay between battery checks, in ms')
        ],
    ota: ota.zigbeeOTA,
};
module.exports = definition;
