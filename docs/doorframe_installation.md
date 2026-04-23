# Doorframe Installation Guide: Safe-Home

This guide details the physical fabrication, coil winding, and hidden installation of the **Safe-Home** security camera inside a standard hollow plastic doorframe molding.

---

## 1. Winding the Custom Narrow Antenna

Traditional circular or square NFC antennas (~45 mm diameter) are too wide to fit inside standard plastic doorframe trim. We construct a **narrow rectangular planar coil**:

```
           25 mm
       ┌───────────┐
       │ ┌───────┐ │
       │ │ ┌───┐ │ │
       │ │ │   │ │ │
75 mm  │ │ │   │ │ │   4 turns of 28 AWG enamelled magnet wire
       │ │ └───┘ │ │   Held together with cyanoacrylate (super glue)
       │ └───────┘ │
       └─────┬─┬───┘
             │ │
         To Tuning Cap (82 pF) & Rectifier
```

1. **Winding Jig:** Cut a piece of rigid cardboard or thin wood to $25 \text{ mm} \times 75 \text{ mm}$.
2. **Coiling:** Wind 4 concentric turns of 28 AWG enamelled copper wire around the perimeter.
3. **Securing:** Apply a few drops of super glue along the edges to keep the turns closely packed, then gently slide the coil off the jig.
4. **Stripping:** Scrape the enamel insulation off both wire ends using sandpaper or a craft blade.
5. **Inductance Verification:** Connect to an LCR meter; the measured inductance should be between $1.65 - 1.85 \ \mu\text{H}$.

---

## 2. Assembling the Rectifier Front-End

Solder the discrete components onto a tiny scrap of protoboard or directly point-to-point:
- Solder an **82 pF C0G ceramic capacitor** in parallel with the two coil leads.
- Solder a **BAT54 Schottky diode** in series with the high side.
- Connect a **10 nF ceramic capacitor** and a **100 kΩ resistor** in parallel from the diode cathode to the ground rail.
- Solder two thin wires:
  - Output signal $\rightarrow$ connect to ESP32-S3 `NFC_TRIGGER_PIN` (GPIO 1).
  - Ground rail $\rightarrow$ connect to ESP32-S3 `GND`.

---

## 3. Embedding inside the Plastic Doorframe

Standard residential and interior doorframes frequently use hollow PVC / vinyl casing trim that snaps or unscrews easily:

```
        Front Exterior View                   Cross-Section (Inside Doorframe)
     ┌───────────────────────┐                 ┌─────────────────────────────┐
     │                       │                 │  Exterior Plastic Molding   │
     │   [2 mm Lens Hole]    │ ──────────────► │    O  <-- Camera Lens       │
     │                       │                 │  ┌───┐                      │
     │                       │                 │  │   │  ESP32-S3 Module     │
     │                       │                 │  └───┘                      │
     │   ░░░░░░░░░░░░░░░░░   │                 │  ┌───┐                      │
     │   ░ [NFC Tap Zone]░   │ ──────────────► │  │   │  Hidden 25x75mm Coil │
     │   ░ (Invisible)   ░   │                 │  └───┘                      │
     │   ░░░░░░░░░░░░░░░░░   │                 │  Hollow Wall Cavity         │
     │                       │                 └─────────────────────────────┘
     └───────────────────────┘
```

1. **Camera Pinhole:** Drill a clean 2 mm hole through the plastic doorframe facing outward at eye/chest level.
2. **Mounting the Camera:** Align the OV2640 lens directly behind the 2 mm hole using hot glue or mounting tape.
3. **Placing the Antenna:** Tape the narrow $25\text{ mm} \times 75\text{ mm}$ coil flat against the interior face of the plastic doorframe below the camera.
4. **Power:** Run a thin micro-USB / USB-C wire down the hollow doorframe channel to a nearby outlet or small 5V LiPo battery pack.
5. **Snap Closed:** Reattach the plastic doorframe molding. The entire system is completely invisible from the outside.

---

## 4. Operation & Testing

1. Approach the doorframe with your smartphone (or an NFC keychain tag).
2. Tap the phone against the doorframe at the marked location.
3. The phone's NFC polling field induces an RF burst in the hidden coil, pulling the trigger pin HIGH.
4. The ESP32-S3 wakes, snaps a snapshot, and delivers a push notification with the photo directly to your phone via Telegram over Wi-Fi in under 2 seconds.
