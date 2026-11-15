# zigbee2mqtt Converters

These converters are external zigbee2mqtt converters for this firmware. They are
not used by the firmware build.

## Compatible with current firmware

Use the `.mjs` files with a current zigbee2mqtt release (targets the
2.14.x / zigbee-herdsman-converters 26.132.x API generation):

| Firmware | Zigbee model ID | Converter |
|---|---|---|
| `light` | `Fixed_TS0501B` | `Fixed_TS0501B.mjs` |
| `switch` | `Fixed_ZBWS01A_TS0041` | `Fixed_ZBWS01A_TS0041.mjs` |
| `IASsensor` | `Fixed_ZG-102ZL` | `Fixed_ZG-102ZL.mjs` |

The model ID in each converter must match `ZCL_BASIC_MODEL_ID` in the active
`device_config/*.h`.

## Latest zigbee2mqtt setup

Zigbee2MQTT 2.x uses ES modules for external converters:

- Place the `.mjs` files in the `external_converters` folder next to
  `configuration.yaml` (or `data/external_converters`).
- External converters are disabled by default in new installations starting with
  zigbee2mqtt 2.11.0. Enable them with:

```yaml
advanced:
  enable_external_js: true
```

- The old `external_converters:` list in `configuration.yaml` is no longer used;
  converters in the folder are loaded automatically.
- Restart zigbee2mqtt after adding or changing a converter.

Converters use `export default` and `import` syntax. They do not use the removed
`zigbee-herdsman-converters/lib/ota` or `lib/extend` modules.

## Behavior exposed to zigbee2mqtt

### Switch (`Fixed_ZBWS01A_TS0041`)

- `action`: `single`, `double`, `triple`, `hold`, `release`
- `battery` and `voltage` (mV)
- Poll Control settings:
  - `checkin_rate`
  - `long_poll_rate`
  - `short_poll_rate`
  - `fast_poll_timeout`
  - `battery_check_rate` (custom attribute `0x6400`)
- OTA updates (`ota: true`)

### Light (`Fixed_TS0501B`)

- On/off, brightness, identify and power-on behavior
- OTA updates (`ota: true`)

### IAS sensor (`Fixed_ZG-102ZL`)

- `contact`
- `illuminance` in lux
- `battery` and `voltage` (mV)
- Poll Control settings:
  - `checkin_rate`
  - `short_poll_rate`
  - `long_poll_rate`
  - `sensor_check_rate` (custom attribute `0x6401`)
- On/Off Switch Configuration:
  - `switch_type`
  - `switch_actions`
- OTA updates (`ota: true`)

## Legacy converters

`legacy/` contains converters for older firmware models and/or the legacy
zigbee2mqtt 1.x converter API. They are not loaded from this subfolder by
zigbee2mqtt, and they depend on packages that no longer exist in current
zigbee-herdsman-converters (`lib/extend`, `lib/ota`, `fz.tuya_on_off_action`).
Keep them only for reference or for an older zigbee2mqtt installation.
