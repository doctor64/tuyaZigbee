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

const definition = {
    zigbeeModel: ['Fixed_TS0041'], // The model ID from: Device with modelID 'lumi.sens' is not supported.
    model: 'Fixed TS0041', // Vendor model number, look on the device for a model number
    vendor: 'DocLab, ex TuYa', // Vendor of the device (only used for documentation and startup logging)        
    description: 'Wireless switch with 1 button', // Description of the device, copy from vendor site. (only used for documentation and startup logging)
    fromZigbee: [fz.command_toggle, fz.battery],
    toZigbee: [tz.factory_reset],
        configure: async (device, coordinatorEndpoint, logger) => {
            const firstEndpoint = device.getEndpoint(1);
            await reporting.bind(firstEndpoint, coordinatorEndpoint, [
                'genPowerCfg' ]);
            const overides = {min: 0, max: 3600, change: 0};
            await reporting.batteryVoltage(firstEndpoint, overides);
            await reporting.batteryPercentageRemaining(firstEndpoint, overides);
        },
    exposes: [e.battery(), e.battery_voltage(), e.action(['toggle','on', 'off', 'brightness_move_up', 'brightness_stop'])],

    ota: ota.zigbeeOTA,
};
module.exports = definition;
