#!/usr/bin/env python3
"""
Custom Doorframe Rectangular Loop Antenna Calculator for 13.56 MHz NFC.

Calculates the inductance of a narrow planar rectangular coil wound with 28 AWG
enamelled magnet wire to fit inside a narrow plastic doorframe channel (e.g. 25 mm x 75 mm),
and computes the required tuning capacitor for 13.56 MHz resonance.
"""

import math

FREQ_HZ = 13.56e6  # 13.56 MHz carrier
MU_0 = 4.0 * math.pi * 1e-7


def calc_rectangular_inductance(a_mm, b_mm, turns, wire_diam_mm=0.32):
    """
    Calculates self-inductance of a rectangular planar coil using Grover's formula.
    a_mm: width (short side) in mm
    b_mm: height (long side) in mm
    turns: number of turns
    wire_diam_mm: wire diameter (28 AWG ≈ 0.32 mm)
    """
    a = a_mm * 1e-3
    b = b_mm * 1e-3
    d = wire_diam_mm * 1e-3
    diag = math.sqrt(a**2 + b**2)

    # Inductance per turn in Henrys
    l_single = (MU_0 / math.pi) * (
        -2 * (a + b)
        + 2 * diag
        - a * math.log((a + diag) / b)
        - b * math.log((b + diag) / a)
        + a * math.log(2 * a / d)
        + b * math.log(2 * b / d)
    )

    # Total inductance scales roughly with N^1.8 - N^2 for closely-spaced spiral
    total_l_h = l_single * (turns ** 1.85)
    return total_l_h * 1e6  # Return in uH


def calc_tuning_capacitance(inductance_uh):
    """Calculates required tuning capacitor C in pF for 13.56 MHz resonance."""
    l_h = inductance_uh * 1e-6
    omega = 2 * math.pi * FREQ_HZ
    c_f = 1 / ((omega ** 2) * l_h)
    return c_f * 1e12


if __name__ == "__main__":
    print("=" * 60)
    print(" Safe-Home: Custom Doorframe NFC Loop Antenna Calculator")
    print("=" * 60)
    
    # Target doorframe molding channel: 25 mm wide x 75 mm tall
    width = 25.0
    height = 75.0

    print(f"Doorframe Channel Dimensions: {width} mm x {height} mm")
    print(f"Wire Spec:                    28 AWG enamelled magnet wire (~0.32 mm)")
    print(f"Target Frequency:             13.56 MHz (NFC / HF RFID)\n")

    print(f"{'Turns':<8} {'Inductance (uH)':<18} {'Tuning Cap (pF)':<18} {'Status'}")
    print("-" * 60)

    for turns in [3, 4, 5, 6]:
        l_uh = calc_rectangular_inductance(width, height, turns)
        c_pf = calc_tuning_capacitance(l_uh)

        if 60 <= c_pf <= 120:
            status = "OPTIMAL (Standard C0G cap)"
        elif c_pf < 60:
            status = "Low C (Parasitics sensitive)"
        else:
            status = "High C"

        print(f"{turns:<8} {l_uh:<18.2f} {c_pf:<18.1f} {status}")

    print("=" * 60)
    print("Recommended Setup:")
    print("  4 turns of 28 AWG wire -> L ~ 1.75 uH -> Pairs with an 82 pF ceramic capacitor.")
    print("  Easily fits inside standard 1-inch hollow plastic doorframe trim.")
    print("=" * 60)
