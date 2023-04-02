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
    zigbeeModel: ['Fixed_TS0501B'], // The model ID from: Device with modelID 'lumi.sens' is not supported.
    model: 'Fixed TS0501B', // Vendor model number, look on the device for a model number
    vendor: 'DocLab, ex TuYa', // Vendor of the device (only used for documentation and startup logging)
    description: 'Zigbee light', // Description of the device, copy from vendor site. (only used for documentation and startup logging)
    // Note that fromZigbee, toZigbee and exposes are missing here since we use extend here.
    // Extend contains a default set of fromZigbee/toZigbee converters and expose for common device types.
    // The following extends are available:
    // - extend.switch
    // - extend.light_onoff_brightness
    // - extend.light_onoff_brightness_colortemp
    // - extend.light_onoff_brightness_color
    // - extend.light_onoff_brightness_colortemp_color
    extend: extend.light_onoff_brightness({noConfigure: true}),
    configure: async (device, coordinatorEndpoint, logger) => {
        await extend.light_onoff_brightness().configure(device, coordinatorEndpoint, logger);
        const endpoint = device.getEndpoint(1);
        await reporting.bind(endpoint, coordinatorEndpoint, ['genOnOff', 'genLevelCtrl']);
        await reporting.onOff(endpoint);
        await reporting.brightness(endpoint);
    },
    ota: ota.zigbeeOTA,
};

module.exports = definition;
