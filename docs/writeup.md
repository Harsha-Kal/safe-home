# Project Write-Up: Safe-Home Invisible Doorframe Security Camera

## 1. Executive Summary
Traditional home security interfaces rely on visible hardware: conspicuous keypads, pin-entry panels, or doorbell buttons that suffer from wear-and-tear, vandalism, and weather degradation. 

**Safe-Home** reimagines physical access and verification as an **invisible, contactless micro-interaction**:
- An ESP32-S3 camera module and a custom-coiled copper wire antenna are hidden entirely within the hollow profile of a plastic doorframe.
- A visitor or homeowner simply taps their smartphone or an NFC keychain against the unmarked doorframe.
- The custom antenna harvests energy from the tap, triggering the ESP32-S3 camera to snap a snapshot and immediately transmit a push notification with the photo directly to the homeowner's smartphone over Wi-Fi.

---

## 2. Antenna Design & Space Constraints

### The Challenge of Physical Constraints
Off-the-shelf RFID/NFC antennas are square or circular (~45 mm to 55 mm diameter). Because standard residential doorframe trim has an interior channel width of only 25 mm to 30 mm, commercial antennas physically cannot fit inside.

### The Solution: Elongated Rectangular Planar Coil
We engineered a custom rectangular planar coil:
- **Dimensions:** $25 \text{ mm}$ width $\times$ $75 \text{ mm}$ length.
- **Conductor:** 4 closely-wound turns of 28 AWG enamelled magnet wire.
- **Inductance:** Using Grover's rectangular loop formulas (`hardware/doorframe_antenna.py`), the calculated self-inductance is:

$$L \approx 1.75 \ \mu\text{H}$$

### Resonant LC Tank Tuning
To harvest maximum energy from the phone's 13.56 MHz carrier frequency, we paired the coil with an $82 \text{ pF}$ C0G ceramic capacitor:

$$f_0 = \frac{1}{2\pi \sqrt{L \cdot C}} = \frac{1}{2\pi \sqrt{(1.75 \times 10^{-6}) \cdot (82 \times 10^{-12})}} \approx 13.27 \text{ MHz}$$

With slight parasitic board capacitance, resonance centers precisely around **13.56 MHz**, providing optimal magnetic coupling through the 2 mm plastic doorframe wall.

---

## 3. Signal Rectification & Camera Trigger

Connecting the coil to a high-speed **BAT54 Schottky diode** ($V_F \approx 0.28\text{ V}$), a **10 nF filter capacitor**, and a **100 kΩ bleeder resistor** creates an envelope detector:
- The phone's alternating 13.56 MHz RF bursts are rectified into a steady **2.2V to 3.1V DC pulse**.
- The pulse feeds directly into the ESP32-S3 interrupt pin (`GPIO 1`).
- The hardware interrupt wakes the processor within milliseconds, bypassing slow polling loops.

---

## 4. Invisible Checkpoint Interaction Design

### Interaction Comparison: Conventional Doorbell vs Safe-Home

| Feature | Traditional Doorbell / Keypad | Safe-Home Invisible Checkpoint |
| :--- | :--- | :--- |
| **Physical Appearance** | Conspicuous plastic box / keypad on wall | **100% Invisible** (hidden inside doorframe) |
| **Vandalism Risk** | Vulnerable to tampering, weathering, theft | **Zero external exposure** |
| **User Action** | Search for button, press button, or type PIN | Natural physical tap of phone/keychain |
| **Telemetry & Security** | Audio chime only | **Instant photographic push notification** |

### Conclusion
By blending antenna physics, embedded microcontrollers, and smartphone notifications, Safe-Home demonstrates that physical computing can make technology fade completely into the architectural background while enhancing security and user experience.
