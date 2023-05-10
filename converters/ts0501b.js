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
        fingerprint: [{modelID: 'TS0501B', manufacturerName: '_TZ3000_4whigl8i'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_4whigl8i'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_9q49basr'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_i680rtja'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_grnwgegn'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_nehayyhx'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_wuheofsg'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_e5t9bfdv'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_19qb27da'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_aurnbfv4'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_4zinq6io'},
            {modelID: 'TS0501B', manufacturerName: '_TZ3210_93gnbdgz'}],
        model: 'TS0501B',
        description: 'Zigbee light',
        vendor: 'TuYa',
        extend: extend.light_onoff_brightness(),
    ota: ota.zigbeeOTA,
};

module.exports = definition;
