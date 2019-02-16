<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.4">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="50" name="dxf" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="250" name="Descript" color="3" fill="1" visible="no" active="no"/>
<layer number="251" name="SMDround" color="12" fill="11" visible="no" active="no"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="avr-mcu">
<packages>
<package name="TQFP44">
<description>&lt;B&gt;Thin Plasic Quad Flat Package&lt;/B&gt;</description>
<wire x1="-4.7" y1="4.3" x2="-4.3" y2="4.7" width="0.254" layer="21"/>
<wire x1="-4.3" y1="4.7" x2="4.3" y2="4.7" width="0.254" layer="21"/>
<wire x1="4.3" y1="4.7" x2="4.7" y2="4.3" width="0.254" layer="21"/>
<wire x1="4.7" y1="4.3" x2="4.7" y2="-4.3" width="0.254" layer="21"/>
<wire x1="4.7" y1="-4.3" x2="4.3" y2="-4.7" width="0.254" layer="21"/>
<wire x1="4.3" y1="-4.7" x2="-4.3" y2="-4.7" width="0.254" layer="21"/>
<wire x1="-4.3" y1="-4.7" x2="-4.7" y2="-4.3" width="0.254" layer="21"/>
<wire x1="-4.7" y1="-4.3" x2="-4.7" y2="4.3" width="0.254" layer="21"/>
<wire x1="-4.7" y1="4.3" x2="-4.3" y2="4.7" width="0.127" layer="51"/>
<wire x1="-4.3" y1="4.7" x2="4.3" y2="4.7" width="0.127" layer="51"/>
<wire x1="4.3" y1="4.7" x2="4.7" y2="4.3" width="0.127" layer="51"/>
<wire x1="4.7" y1="4.3" x2="4.7" y2="-4.3" width="0.127" layer="51"/>
<wire x1="4.7" y1="-4.3" x2="4.3" y2="-4.7" width="0.127" layer="51"/>
<wire x1="4.3" y1="-4.7" x2="-4.3" y2="-4.7" width="0.127" layer="51"/>
<wire x1="-4.3" y1="-4.7" x2="-4.7" y2="-4.3" width="0.127" layer="51"/>
<wire x1="-4.7" y1="-4.3" x2="-4.7" y2="4.3" width="0.127" layer="51"/>
<circle x="-3.8" y="3.8" radius="0.4" width="0.254" layer="21"/>
<circle x="-3.8" y="3.8" radius="0.4" width="0.127" layer="51"/>
<smd name="1" x="-5.8" y="4" dx="1.5" dy="0.5" layer="1"/>
<smd name="2" x="-5.8" y="3.2" dx="1.5" dy="0.5" layer="1"/>
<smd name="3" x="-5.8" y="2.4" dx="1.5" dy="0.5" layer="1"/>
<smd name="4" x="-5.8" y="1.6" dx="1.5" dy="0.5" layer="1"/>
<smd name="5" x="-5.8" y="0.8" dx="1.5" dy="0.5" layer="1"/>
<smd name="6" x="-5.8" y="0" dx="1.5" dy="0.5" layer="1"/>
<smd name="7" x="-5.8" y="-0.8" dx="1.5" dy="0.5" layer="1"/>
<smd name="8" x="-5.8" y="-1.6" dx="1.5" dy="0.5" layer="1"/>
<smd name="9" x="-5.8" y="-2.4" dx="1.5" dy="0.5" layer="1"/>
<smd name="10" x="-5.8" y="-3.2" dx="1.5" dy="0.5" layer="1"/>
<smd name="11" x="-5.8" y="-4" dx="1.5" dy="0.5" layer="1"/>
<smd name="12" x="-4" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="13" x="-3.2" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="14" x="-2.4" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="15" x="-1.6" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="16" x="-0.8" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="17" x="0" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="18" x="0.8" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="19" x="1.6" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="20" x="2.4" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="21" x="3.2" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="22" x="4" y="-5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="23" x="5.8" y="-4" dx="1.5" dy="0.5" layer="1"/>
<smd name="24" x="5.8" y="-3.2" dx="1.5" dy="0.5" layer="1"/>
<smd name="25" x="5.8" y="-2.4" dx="1.5" dy="0.5" layer="1"/>
<smd name="26" x="5.8" y="-1.6" dx="1.5" dy="0.5" layer="1"/>
<smd name="27" x="5.8" y="-0.8" dx="1.5" dy="0.5" layer="1"/>
<smd name="28" x="5.8" y="0" dx="1.5" dy="0.5" layer="1"/>
<smd name="29" x="5.8" y="0.8" dx="1.5" dy="0.5" layer="1"/>
<smd name="30" x="5.8" y="1.6" dx="1.5" dy="0.5" layer="1"/>
<smd name="31" x="5.8" y="2.4" dx="1.5" dy="0.5" layer="1"/>
<smd name="32" x="5.8" y="3.2" dx="1.5" dy="0.5" layer="1"/>
<smd name="33" x="5.8" y="4" dx="1.5" dy="0.5" layer="1"/>
<smd name="34" x="4" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="35" x="3.2" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="36" x="2.4" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="37" x="1.6" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="38" x="0.8" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="39" x="0" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="40" x="-0.8" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="41" x="-1.6" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="42" x="-2.4" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="43" x="-3.2" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<smd name="44" x="-4" y="5.8" dx="0.5" dy="1.5" layer="1"/>
<text x="-3.81" y="6.985" size="1.778" layer="25" font="vector">&gt;NAME</text>
<text x="-4.445" y="-8.7551" size="1.778" layer="27" font="vector">&gt;VALUE</text>
<text x="-2.5" y="3.4" size="0.762" layer="51" font="vector" ratio="12">&gt;NAME</text>
<rectangle x1="-6.1" y1="3.8" x2="-4.7" y2="4.2" layer="51"/>
<rectangle x1="-4.2" y1="-6.1" x2="-3.8" y2="-4.7" layer="51"/>
<rectangle x1="4.7" y1="-4.2" x2="6.1" y2="-3.8" layer="51"/>
<rectangle x1="3.8" y1="4.7" x2="4.2" y2="6.1" layer="51"/>
<rectangle x1="-6.1" y1="3" x2="-4.7" y2="3.4" layer="51"/>
<rectangle x1="-6.1" y1="2.2" x2="-4.7" y2="2.6" layer="51"/>
<rectangle x1="-6.1" y1="1.4" x2="-4.7" y2="1.8" layer="51"/>
<rectangle x1="-6.1" y1="0.6" x2="-4.7" y2="1" layer="51"/>
<rectangle x1="-6.1" y1="-0.2" x2="-4.7" y2="0.2" layer="51"/>
<rectangle x1="-6.1" y1="-1" x2="-4.7" y2="-0.6" layer="51"/>
<rectangle x1="-6.1" y1="-1.8" x2="-4.7" y2="-1.4" layer="51"/>
<rectangle x1="-6.1" y1="-2.6" x2="-4.7" y2="-2.2" layer="51"/>
<rectangle x1="-6.1" y1="-3.4" x2="-4.7" y2="-3" layer="51"/>
<rectangle x1="-6.1" y1="-4.2" x2="-4.7" y2="-3.8" layer="51"/>
<rectangle x1="-3.4" y1="-6.1" x2="-3" y2="-4.7" layer="51"/>
<rectangle x1="-2.6" y1="-6.1" x2="-2.2" y2="-4.7" layer="51"/>
<rectangle x1="-1.8" y1="-6.1" x2="-1.4" y2="-4.7" layer="51"/>
<rectangle x1="-1" y1="-6.1" x2="-0.6" y2="-4.7" layer="51"/>
<rectangle x1="-0.2" y1="-6.1" x2="0.2" y2="-4.7" layer="51"/>
<rectangle x1="0.6" y1="-6.1" x2="1" y2="-4.7" layer="51"/>
<rectangle x1="1.4" y1="-6.1" x2="1.8" y2="-4.7" layer="51"/>
<rectangle x1="2.2" y1="-6.1" x2="2.6" y2="-4.7" layer="51"/>
<rectangle x1="3" y1="-6.1" x2="3.4" y2="-4.7" layer="51"/>
<rectangle x1="3.8" y1="-6.1" x2="4.2" y2="-4.7" layer="51"/>
<rectangle x1="4.7" y1="-3.4" x2="6.1" y2="-3" layer="51"/>
<rectangle x1="4.7" y1="-2.6" x2="6.1" y2="-2.2" layer="51"/>
<rectangle x1="4.7" y1="-1.8" x2="6.1" y2="-1.4" layer="51"/>
<rectangle x1="4.7" y1="-1" x2="6.1" y2="-0.6" layer="51"/>
<rectangle x1="4.7" y1="-0.2" x2="6.1" y2="0.2" layer="51"/>
<rectangle x1="4.7" y1="0.6" x2="6.1" y2="1" layer="51"/>
<rectangle x1="4.7" y1="1.4" x2="6.1" y2="1.8" layer="51"/>
<rectangle x1="4.7" y1="2.2" x2="6.1" y2="2.6" layer="51"/>
<rectangle x1="4.7" y1="3" x2="6.1" y2="3.4" layer="51"/>
<rectangle x1="4.7" y1="3.8" x2="6.1" y2="4.2" layer="51"/>
<rectangle x1="3" y1="4.7" x2="3.4" y2="6.1" layer="51"/>
<rectangle x1="2.2" y1="4.7" x2="2.6" y2="6.1" layer="51"/>
<rectangle x1="1.4" y1="4.7" x2="1.8" y2="6.1" layer="51"/>
<rectangle x1="0.6" y1="4.7" x2="1" y2="6.1" layer="51"/>
<rectangle x1="-0.2" y1="4.7" x2="0.2" y2="6.1" layer="51"/>
<rectangle x1="-1" y1="4.7" x2="-0.6" y2="6.1" layer="51"/>
<rectangle x1="-1.8" y1="4.7" x2="-1.4" y2="6.1" layer="51"/>
<rectangle x1="-2.6" y1="4.7" x2="-2.2" y2="6.1" layer="51"/>
<rectangle x1="-3.4" y1="4.7" x2="-3" y2="6.1" layer="51"/>
<rectangle x1="-4.2" y1="4.7" x2="-3.8" y2="6.1" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="MEGA164P/324P/644P-SYM">
<wire x1="-17.78" y1="35.56" x2="17.78" y2="35.56" width="0.254" layer="94"/>
<wire x1="17.78" y1="35.56" x2="17.78" y2="-38.1" width="0.254" layer="94"/>
<wire x1="17.78" y1="-38.1" x2="-17.78" y2="-38.1" width="0.254" layer="94"/>
<wire x1="-17.78" y1="-38.1" x2="-17.78" y2="35.56" width="0.254" layer="94"/>
<text x="-17.78" y="36.322" size="1.778" layer="95">&gt;NAME</text>
<text x="-17.78" y="-40.64" size="1.778" layer="96">&gt;VALUE</text>
<pin name="PA7(ADC7)" x="-22.86" y="-15.24" length="middle"/>
<pin name="PA6(ADC6)" x="-22.86" y="-17.78" length="middle"/>
<pin name="PA5(ADC5)" x="-22.86" y="-20.32" length="middle"/>
<pin name="PA4(ADC4)" x="-22.86" y="-22.86" length="middle"/>
<pin name="PA3(ADC3)" x="-22.86" y="-25.4" length="middle"/>
<pin name="PA2(ADC2)" x="-22.86" y="-27.94" length="middle"/>
<pin name="PA1(ADC1)" x="-22.86" y="-30.48" length="middle"/>
<pin name="PA0(ADC0)" x="-22.86" y="-33.02" length="middle"/>
<pin name="PB7(SCK)" x="22.86" y="30.48" length="middle" rot="R180"/>
<pin name="PB6(MISO)" x="22.86" y="27.94" length="middle" rot="R180"/>
<pin name="PB5(MOSI)" x="22.86" y="25.4" length="middle" rot="R180"/>
<pin name="PB4(SS/OC0B)" x="22.86" y="22.86" length="middle" rot="R180"/>
<pin name="PB3(AIN1/OC0A)" x="22.86" y="20.32" length="middle" rot="R180"/>
<pin name="PB2(AIN0/INT2)" x="22.86" y="17.78" length="middle" rot="R180"/>
<pin name="PB1(T1/CLKO)" x="22.86" y="15.24" length="middle" rot="R180"/>
<pin name="PB0(T0/XCK)" x="22.86" y="12.7" length="middle" rot="R180"/>
<pin name="PC7(TOSC2)" x="22.86" y="7.62" length="middle" rot="R180"/>
<pin name="PC6(TOSC1)" x="22.86" y="5.08" length="middle" rot="R180"/>
<pin name="PC5(TDI)" x="22.86" y="2.54" length="middle" rot="R180"/>
<pin name="PC4(TDO)" x="22.86" y="0" length="middle" rot="R180"/>
<pin name="PC3(TMS)" x="22.86" y="-2.54" length="middle" rot="R180"/>
<pin name="PC2(TCK)" x="22.86" y="-5.08" length="middle" rot="R180"/>
<pin name="PC1(SDA)" x="22.86" y="-7.62" length="middle" rot="R180"/>
<pin name="PC0(SCL)" x="22.86" y="-10.16" length="middle" rot="R180"/>
<pin name="AGND" x="-22.86" y="-10.16" length="middle" direction="pwr"/>
<pin name="AVCC" x="-22.86" y="-2.54" length="middle" direction="pwr"/>
<pin name="AREF" x="-22.86" y="-5.08" length="middle" direction="pwr"/>
<pin name="XTAL1" x="-22.86" y="20.32" length="middle"/>
<pin name="XTAL2" x="-22.86" y="25.4" length="middle"/>
<pin name="VCC@1" x="-22.86" y="12.7" length="middle" direction="pwr"/>
<pin name="GND@1" x="-22.86" y="5.08" length="middle" direction="pwr"/>
<pin name="/RESET" x="-22.86" y="30.48" length="middle" direction="in" function="dot"/>
<pin name="PD0(RXD0)" x="22.86" y="-33.02" length="middle" rot="R180"/>
<pin name="PD1(TXD0)" x="22.86" y="-30.48" length="middle" rot="R180"/>
<pin name="PD2(INT0/RXD1)" x="22.86" y="-27.94" length="middle" rot="R180"/>
<pin name="PD3(INT1/TXD1)" x="22.86" y="-25.4" length="middle" rot="R180"/>
<pin name="PD4(XCK1/OC1B)" x="22.86" y="-22.86" length="middle" rot="R180"/>
<pin name="PD5(OC1A)" x="22.86" y="-20.32" length="middle" rot="R180"/>
<pin name="PD6(ICP/OC2B)" x="22.86" y="-17.78" length="middle" rot="R180"/>
<pin name="PD7(OC2A)" x="22.86" y="-15.24" length="middle" rot="R180"/>
<pin name="VCC@3" x="-22.86" y="15.24" length="middle" direction="pwr"/>
<pin name="VCC@2" x="-22.86" y="10.16" length="middle" direction="pwr"/>
<pin name="GND@3" x="-22.86" y="7.62" length="middle" direction="pwr"/>
<pin name="GND@2" x="-22.86" y="2.54" length="middle" direction="pwr"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="ATMEGA644P" uservalue="yes">
<description>&lt;b&gt;Microcontroller&lt;/b&gt;
&lt;p&gt;64kB Flash, 4kB SRAM, 2kB EEPROM</description>
<gates>
<gate name="G$1" symbol="MEGA164P/324P/644P-SYM" x="0" y="0"/>
</gates>
<devices>
<device name="" package="TQFP44">
<connects>
<connect gate="G$1" pin="/RESET" pad="4"/>
<connect gate="G$1" pin="AGND" pad="28"/>
<connect gate="G$1" pin="AREF" pad="29"/>
<connect gate="G$1" pin="AVCC" pad="27"/>
<connect gate="G$1" pin="GND@1" pad="6"/>
<connect gate="G$1" pin="GND@2" pad="18"/>
<connect gate="G$1" pin="GND@3" pad="39"/>
<connect gate="G$1" pin="PA0(ADC0)" pad="37"/>
<connect gate="G$1" pin="PA1(ADC1)" pad="36"/>
<connect gate="G$1" pin="PA2(ADC2)" pad="35"/>
<connect gate="G$1" pin="PA3(ADC3)" pad="34"/>
<connect gate="G$1" pin="PA4(ADC4)" pad="33"/>
<connect gate="G$1" pin="PA5(ADC5)" pad="32"/>
<connect gate="G$1" pin="PA6(ADC6)" pad="31"/>
<connect gate="G$1" pin="PA7(ADC7)" pad="30"/>
<connect gate="G$1" pin="PB0(T0/XCK)" pad="40"/>
<connect gate="G$1" pin="PB1(T1/CLKO)" pad="41"/>
<connect gate="G$1" pin="PB2(AIN0/INT2)" pad="42"/>
<connect gate="G$1" pin="PB3(AIN1/OC0A)" pad="43"/>
<connect gate="G$1" pin="PB4(SS/OC0B)" pad="44"/>
<connect gate="G$1" pin="PB5(MOSI)" pad="1"/>
<connect gate="G$1" pin="PB6(MISO)" pad="2"/>
<connect gate="G$1" pin="PB7(SCK)" pad="3"/>
<connect gate="G$1" pin="PC0(SCL)" pad="19"/>
<connect gate="G$1" pin="PC1(SDA)" pad="20"/>
<connect gate="G$1" pin="PC2(TCK)" pad="21"/>
<connect gate="G$1" pin="PC3(TMS)" pad="22"/>
<connect gate="G$1" pin="PC4(TDO)" pad="23"/>
<connect gate="G$1" pin="PC5(TDI)" pad="24"/>
<connect gate="G$1" pin="PC6(TOSC1)" pad="25"/>
<connect gate="G$1" pin="PC7(TOSC2)" pad="26"/>
<connect gate="G$1" pin="PD0(RXD0)" pad="9"/>
<connect gate="G$1" pin="PD1(TXD0)" pad="10"/>
<connect gate="G$1" pin="PD2(INT0/RXD1)" pad="11"/>
<connect gate="G$1" pin="PD3(INT1/TXD1)" pad="12"/>
<connect gate="G$1" pin="PD4(XCK1/OC1B)" pad="13"/>
<connect gate="G$1" pin="PD5(OC1A)" pad="14"/>
<connect gate="G$1" pin="PD6(ICP/OC2B)" pad="15"/>
<connect gate="G$1" pin="PD7(OC2A)" pad="16"/>
<connect gate="G$1" pin="VCC@1" pad="5"/>
<connect gate="G$1" pin="VCC@2" pad="17"/>
<connect gate="G$1" pin="VCC@3" pad="38"/>
<connect gate="G$1" pin="XTAL1" pad="8"/>
<connect gate="G$1" pin="XTAL2" pad="7"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="frame">
<packages>
</packages>
<symbols>
<symbol name="DINA4_L">
<frame x1="0" y1="0" x2="264.16" y2="180.34" columns="5" rows="4" layer="94" border-right="no" border-bottom="no"/>
</symbol>
<symbol name="DOCFIELD">
<wire x1="0" y1="0" x2="71.12" y2="0" width="0.1016" layer="94"/>
<wire x1="101.6" y1="15.24" x2="87.63" y2="15.24" width="0.1016" layer="94"/>
<wire x1="0" y1="0" x2="0" y2="5.08" width="0.1016" layer="94"/>
<wire x1="0" y1="5.08" x2="71.12" y2="5.08" width="0.1016" layer="94"/>
<wire x1="0" y1="5.08" x2="0" y2="15.24" width="0.1016" layer="94"/>
<wire x1="101.6" y1="15.24" x2="101.6" y2="5.08" width="0.1016" layer="94"/>
<wire x1="71.12" y1="5.08" x2="71.12" y2="0" width="0.1016" layer="94"/>
<wire x1="71.12" y1="5.08" x2="87.63" y2="5.08" width="0.1016" layer="94"/>
<wire x1="71.12" y1="0" x2="101.6" y2="0" width="0.1016" layer="94"/>
<wire x1="87.63" y1="15.24" x2="87.63" y2="5.08" width="0.1016" layer="94"/>
<wire x1="87.63" y1="15.24" x2="0" y2="15.24" width="0.1016" layer="94"/>
<wire x1="87.63" y1="5.08" x2="101.6" y2="5.08" width="0.1016" layer="94"/>
<wire x1="101.6" y1="5.08" x2="101.6" y2="0" width="0.1016" layer="94"/>
<wire x1="0" y1="15.24" x2="0" y2="22.86" width="0.1016" layer="94"/>
<wire x1="101.6" y1="35.56" x2="0" y2="35.56" width="0.1016" layer="94"/>
<wire x1="101.6" y1="35.56" x2="101.6" y2="22.86" width="0.1016" layer="94"/>
<wire x1="0" y1="22.86" x2="101.6" y2="22.86" width="0.1016" layer="94"/>
<wire x1="0" y1="22.86" x2="0" y2="35.56" width="0.1016" layer="94"/>
<wire x1="101.6" y1="22.86" x2="101.6" y2="15.24" width="0.1016" layer="94"/>
<text x="1.27" y="1.27" size="2.54" layer="94" font="vector">Date:</text>
<text x="12.7" y="1.27" size="2.54" layer="94" font="vector">&gt;LAST_DATE_TIME</text>
<text x="72.39" y="1.27" size="2.54" layer="94" font="vector">Sheet:</text>
<text x="86.36" y="1.27" size="2.54" layer="94" font="vector">&gt;SHEET</text>
<text x="88.9" y="11.43" size="2.54" layer="94" font="vector">REV:</text>
<text x="1.27" y="19.05" size="2.54" layer="94" font="vector">TITLE:</text>
<text x="1.27" y="11.43" size="2.54" layer="94" font="vector">Document Number:</text>
<text x="17.78" y="19.05" size="2.54" layer="94" font="vector">&gt;DRAWING_NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="DINA4_L" prefix="FRAME" uservalue="yes">
<description>&lt;b&gt;FRAME&lt;/b&gt;&lt;p&gt;
DIN A4, landscape with extra doc field</description>
<gates>
<gate name="G$1" symbol="DINA4_L" x="0" y="0"/>
<gate name="G$2" symbol="DOCFIELD" x="162.56" y="0" addlevel="must"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="_newbylars">
<packages>
<package name="R0805">
<description>&lt;b&gt;Chip resistor 0805 reflow&lt;/b&gt;&lt;p&gt;
Size 0805 (IPC 2012) chip resistor. Reflow soldering footprint.</description>
<wire x1="-1.973" y1="0.983" x2="1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="-0.983" x2="-1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.973" y1="-0.983" x2="-1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="0.983" x2="1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.05" y1="0.7" x2="1.05" y2="0.7" width="0.0508" layer="51"/>
<wire x1="1.05" y1="0.7" x2="1.05" y2="-0.7" width="0.0508" layer="51"/>
<wire x1="1.05" y1="-0.7" x2="-1.05" y2="-0.7" width="0.0508" layer="51"/>
<wire x1="-1.05" y1="-0.7" x2="-1.05" y2="0.7" width="0.0508" layer="51"/>
<smd name="1" x="-0.95" y="0" dx="1.3" dy="1.5" layer="1"/>
<smd name="2" x="0.95" y="0" dx="1.3" dy="1.5" layer="1"/>
<text x="-1.27" y="1.27" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.54" size="1.27" layer="27">&gt;VALUE</text>
<text x="-0.9" y="-0.2" size="0.4064" layer="51">&gt;NAME</text>
<rectangle x1="-0.1001" y1="-0.4001" x2="0.1001" y2="0.4001" layer="35"/>
</package>
<package name="DIL08">
<description>&lt;h1&gt;Dual In Line Package 300mil, 8-pin&lt;/h1&gt;</description>
<wire x1="-5.08" y1="2.032" x2="-5.08" y2="1.016" width="0.2032" layer="21"/>
<wire x1="-5.08" y1="-2.032" x2="-5.08" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="-5.08" y1="1.016" x2="-5.08" y2="-1.016" width="0.2032" layer="21" curve="-180"/>
<wire x1="-5.08" y1="2.921" x2="-5.08" y2="1.016" width="0.127" layer="51"/>
<wire x1="-5.08" y1="1.016" x2="-5.08" y2="-1.016" width="0.254" layer="51" curve="-180"/>
<wire x1="-5.08" y1="-2.921" x2="-5.08" y2="-1.016" width="0.127" layer="51"/>
<wire x1="-5.08" y1="-2.921" x2="5.08" y2="-2.921" width="0.127" layer="51"/>
<wire x1="5.08" y1="2.921" x2="-5.08" y2="2.921" width="0.127" layer="51"/>
<wire x1="5.08" y1="2.921" x2="5.08" y2="-2.921" width="0.127" layer="51"/>
<wire x1="-5.08" y1="2.032" x2="5.08" y2="2.032" width="0.2032" layer="21"/>
<wire x1="5.08" y1="2.032" x2="5.08" y2="-2.032" width="0.2032" layer="21"/>
<wire x1="5.08" y1="-2.032" x2="-5.08" y2="-2.032" width="0.2032" layer="21"/>
<pad name="1" x="-3.81" y="-3.81" drill="0.8" shape="long" rot="R90"/>
<pad name="2" x="-1.27" y="-3.81" drill="0.8" shape="long" rot="R90"/>
<pad name="3" x="1.27" y="-3.81" drill="0.8" shape="long" rot="R90"/>
<pad name="4" x="3.81" y="-3.81" drill="0.8" shape="long" rot="R90"/>
<pad name="5" x="3.81" y="3.81" drill="0.8" shape="long" rot="R90"/>
<pad name="6" x="1.27" y="3.81" drill="0.8" shape="long" rot="R90"/>
<pad name="7" x="-1.27" y="3.81" drill="0.8" shape="long" rot="R90"/>
<pad name="8" x="-3.81" y="3.81" drill="0.8" shape="long" rot="R90"/>
<text x="-5.461" y="-2.921" size="1.27" layer="25" font="vector" ratio="16" rot="R90">&gt;NAME</text>
<text x="6.985" y="-2.921" size="1.27" layer="27" font="vector" ratio="16" rot="R90">&gt;VALUE</text>
<text x="-2.286" y="-0.762" size="1.27" layer="51" font="vector" ratio="10">&gt;NAME</text>
</package>
<package name="RTRIM3339P">
<description>&lt;b&gt;Trimm resistor&lt;/b&gt; BOURNS&lt;p&gt;
Cermet MIL-R-22097</description>
<circle x="0" y="0" radius="3.81" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="1.4199" width="0.1524" layer="21"/>
<pad name="1" x="-2.54" y="0" drill="0.6096"/>
<pad name="2" x="0" y="-2.54" drill="0.6096"/>
<pad name="3" x="2.54" y="0" drill="0.6096"/>
<text x="-2.515" y="4.205" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.515" y="-5.605" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.016" y1="-0.254" x2="1.016" y2="0.254" layer="21"/>
<rectangle x1="-0.254" y1="-1.016" x2="0.254" y2="1.016" layer="21"/>
</package>
<package name="0207/7">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0207, grid 7.5 mm</description>
<wire x1="-3.81" y1="0" x2="-3.429" y2="0" width="0.6096" layer="51"/>
<wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-3.175" y2="0.889" width="0.1524" layer="51"/>
<wire x1="-2.921" y1="1.143" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="1.016" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="-1.143" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="-1.016" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="-2.413" y2="1.016" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="2.921" y1="1.143" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.921" y1="-1.143" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-0.889" x2="3.175" y2="0.889" width="0.1524" layer="51"/>
<wire x1="3.429" y1="0" x2="3.81" y2="0" width="0.6096" layer="51"/>
<pad name="1" x="-3.81" y="0" drill="0.8128" shape="long"/>
<pad name="2" x="3.81" y="0" drill="0.8128" shape="long"/>
<text x="-2.54" y="1.397" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.286" y="-0.5588" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-3.429" y1="-0.3048" x2="-3.175" y2="0.3048" layer="51"/>
<rectangle x1="3.175" y1="-0.3048" x2="3.429" y2="0.3048" layer="51"/>
</package>
<package name="DIN5_FEM_UNIV">
<description>Female &lt;b&gt;CONNECTOR&lt;/b&gt;&lt;p&gt;
5 pins 90 deg. with shield, horizontal (DIN 41524)</description>
<wire x1="-10.668" y1="-7.62" x2="9.779" y2="-7.62" width="0.2032" layer="21"/>
<wire x1="6.096" y1="8.636" x2="7.112" y2="6.35" width="0.1524" layer="51"/>
<wire x1="8.763" y1="6.35" x2="7.112" y2="6.35" width="0.1524" layer="51"/>
<wire x1="9.779" y1="-7.62" x2="9.779" y2="-6.858" width="0.2032" layer="21"/>
<wire x1="10.668" y1="-6.858" x2="9.779" y2="-6.858" width="0.2032" layer="21"/>
<wire x1="10.668" y1="-6.858" x2="10.668" y2="-6.35" width="0.2032" layer="21"/>
<wire x1="10.668" y1="-6.35" x2="9.779" y2="-6.35" width="0.2032" layer="21"/>
<wire x1="9.779" y1="-5.715" x2="9.779" y2="-6.35" width="0.2032" layer="21"/>
<wire x1="7.112" y1="6.35" x2="6.35" y2="6.35" width="0.1524" layer="51"/>
<wire x1="6.35" y1="6.35" x2="3.175" y2="6.35" width="0.1524" layer="51"/>
<wire x1="6.096" y1="8.636" x2="1.905" y2="8.636" width="0.1524" layer="51"/>
<wire x1="-1.905" y1="8.636" x2="-6.223" y2="8.636" width="0.1524" layer="51"/>
<wire x1="-6.35" y1="6.477" x2="-6.35" y2="2.54" width="0.1524" layer="51"/>
<wire x1="-6.35" y1="6.477" x2="-7.112" y2="6.477" width="0.1524" layer="51"/>
<wire x1="6.35" y1="6.35" x2="6.35" y2="2.54" width="0.1524" layer="51"/>
<wire x1="-10.668" y1="-7.62" x2="-10.668" y2="-6.858" width="0.2032" layer="21"/>
<wire x1="-9.525" y1="-6.858" x2="-10.668" y2="-6.858" width="0.2032" layer="21"/>
<wire x1="-9.525" y1="-6.858" x2="-9.525" y2="-6.35" width="0.2032" layer="21"/>
<wire x1="-10.668" y1="-6.35" x2="-9.525" y2="-6.35" width="0.2032" layer="21"/>
<wire x1="-10.668" y1="-6.35" x2="-10.668" y2="-5.715" width="0.2032" layer="21"/>
<wire x1="-8.89" y1="-5.715" x2="-10.668" y2="-5.715" width="0.2032" layer="21"/>
<wire x1="-7.112" y1="6.477" x2="-6.223" y2="8.636" width="0.1524" layer="51"/>
<wire x1="-8.763" y1="6.477" x2="-7.112" y2="6.477" width="0.1524" layer="51"/>
<wire x1="-3.175" y1="6.477" x2="-6.35" y2="6.477" width="0.1524" layer="51"/>
<wire x1="-8.763" y1="-5.715" x2="-6.223" y2="-5.715" width="0.2032" layer="21"/>
<wire x1="-1.397" y1="-5.715" x2="1.397" y2="-5.715" width="0.2032" layer="21"/>
<wire x1="6.223" y1="-5.715" x2="9.779" y2="-5.715" width="0.2032" layer="21"/>
<wire x1="-8.89" y1="6.35" x2="-1.27" y2="6.35" width="0.2032" layer="21"/>
<wire x1="1.27" y1="6.35" x2="8.89" y2="6.35" width="0.2032" layer="21"/>
<wire x1="-8.89" y1="6.35" x2="-8.89" y2="1.27" width="0.2032" layer="21"/>
<wire x1="-8.89" y1="-1.27" x2="-8.89" y2="-5.715" width="0.2032" layer="21"/>
<wire x1="8.89" y1="6.35" x2="8.89" y2="1.27" width="0.2032" layer="21"/>
<wire x1="8.89" y1="-1.27" x2="8.89" y2="-5.715" width="0.2032" layer="21"/>
<wire x1="-10.668" y1="-7.62" x2="9.779" y2="-7.62" width="0.1524" layer="51"/>
<wire x1="9.779" y1="-7.62" x2="9.779" y2="-6.858" width="0.1524" layer="51"/>
<wire x1="10.668" y1="-6.858" x2="9.779" y2="-6.858" width="0.1524" layer="51"/>
<wire x1="10.668" y1="-6.858" x2="10.668" y2="-6.35" width="0.1524" layer="51"/>
<wire x1="10.668" y1="-6.35" x2="9.779" y2="-6.35" width="0.1524" layer="51"/>
<wire x1="9.779" y1="-5.715" x2="9.779" y2="-6.35" width="0.1524" layer="51"/>
<wire x1="-10.668" y1="-7.62" x2="-10.668" y2="-6.858" width="0.1524" layer="51"/>
<wire x1="-9.525" y1="-6.858" x2="-10.668" y2="-6.858" width="0.1524" layer="51"/>
<wire x1="-9.525" y1="-6.858" x2="-9.525" y2="-6.35" width="0.1524" layer="51"/>
<wire x1="-10.668" y1="-6.35" x2="-9.525" y2="-6.35" width="0.1524" layer="51"/>
<wire x1="-10.668" y1="-6.35" x2="-10.668" y2="-5.715" width="0.1524" layer="51"/>
<wire x1="-8.89" y1="-5.715" x2="-10.668" y2="-5.715" width="0.1524" layer="51"/>
<wire x1="-8.763" y1="-5.715" x2="-6.477" y2="-5.715" width="0.1524" layer="51"/>
<wire x1="-6.477" y1="-5.715" x2="-6.223" y2="-5.715" width="0.1524" layer="51"/>
<wire x1="-6.223" y1="-5.715" x2="9.779" y2="-5.715" width="0.1524" layer="51"/>
<wire x1="-8.89" y1="6.35" x2="-1.27" y2="6.35" width="0.127" layer="51"/>
<wire x1="1.27" y1="6.35" x2="8.89" y2="6.35" width="0.127" layer="51"/>
<wire x1="-8.89" y1="6.35" x2="-8.89" y2="-5.715" width="0.127" layer="51"/>
<wire x1="8.89" y1="6.35" x2="8.89" y2="-5.715" width="0.127" layer="51"/>
<pad name="2" x="0" y="5.08" drill="1.3208" shape="long" rot="R90"/>
<pad name="1" x="7.62" y="5.08" drill="1.3208" shape="octagon"/>
<pad name="3" x="-7.62" y="5.08" drill="1.3208" shape="octagon"/>
<pad name="4" x="5.08" y="7.62" drill="1.3208" shape="octagon"/>
<pad name="5" x="-5.08" y="7.62" drill="1.3208" shape="octagon"/>
<pad name="PE@2" x="2.54" y="-5.08" drill="1.3208" shape="octagon"/>
<pad name="PE@3" x="-2.54" y="-5.08" drill="1.3208" shape="octagon"/>
<pad name="PE@4" x="-5.08" y="-5.08" drill="1.3208" shape="octagon"/>
<pad name="PE@1" x="5.08" y="-5.08" drill="1.3208" shape="octagon"/>
<text x="-6.096" y="9.144" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="7.62" y="6.604" size="1.27" layer="21" font="vector" ratio="16">1</text>
<text x="-5.08" y="0" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="7.62" y="6.604" size="1.27" layer="51" font="vector" ratio="16">1</text>
<rectangle x1="3.175" y1="4.572" x2="8.001" y2="5.588" layer="51"/>
<rectangle x1="0.635" y1="7.112" x2="5.461" y2="8.128" layer="51"/>
<rectangle x1="-5.461" y1="7.112" x2="-0.635" y2="8.128" layer="51"/>
<rectangle x1="-8.001" y1="4.572" x2="-3.175" y2="5.588" layer="51"/>
<hole x="7.62" y="0" drill="2.794"/>
<hole x="-7.62" y="0" drill="2.794"/>
</package>
<package name="DO34Z7">
<description>&lt;B&gt;DIODE&lt;/B&gt;&lt;p&gt;
diameter 1.8 mm, horizontal, grid 7.62 mm</description>
<wire x1="-1.524" y1="-0.889" x2="1.524" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="1.524" y1="0.889" x2="-1.524" y2="0.889" width="0.1524" layer="21"/>
<wire x1="1.524" y1="-0.889" x2="1.524" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="0.889" x2="-1.524" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="3.81" y1="0" x2="2.921" y2="0" width="0.508" layer="51"/>
<wire x1="-3.81" y1="0" x2="-2.921" y2="0" width="0.508" layer="51"/>
<pad name="C" x="-3.81" y="0" drill="0.8128" shape="long"/>
<pad name="A" x="3.81" y="0" drill="0.8128" shape="long"/>
<text x="-1.524" y="1.143" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.524" y="-2.413" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.143" y1="-0.889" x2="-0.762" y2="0.889" layer="21"/>
<rectangle x1="-2.286" y1="-0.254" x2="-1.524" y2="0.254" layer="21"/>
<rectangle x1="1.524" y1="-0.254" x2="2.286" y2="0.254" layer="21"/>
</package>
<package name="SMB">
<description>&lt;B&gt;DIODE&lt;/B&gt;</description>
<wire x1="-2.2606" y1="-1.905" x2="-2.2606" y2="1.905" width="0.1016" layer="51"/>
<wire x1="2.2606" y1="-1.905" x2="2.2606" y2="1.905" width="0.1016" layer="51"/>
<wire x1="-2.286" y1="1.397" x2="-2.286" y2="1.905" width="0.127" layer="21"/>
<wire x1="-2.286" y1="1.905" x2="2.286" y2="1.905" width="0.127" layer="21"/>
<wire x1="2.286" y1="1.905" x2="2.286" y2="1.397" width="0.127" layer="21"/>
<wire x1="-2.286" y1="-1.397" x2="-2.286" y2="-1.905" width="0.127" layer="21"/>
<wire x1="-2.286" y1="-1.905" x2="2.286" y2="-1.905" width="0.127" layer="21"/>
<wire x1="2.286" y1="-1.905" x2="2.286" y2="-1.397" width="0.127" layer="21"/>
<smd name="C" x="-2.2" y="0" dx="2.4" dy="2.4" layer="1"/>
<smd name="A" x="2.2" y="0" dx="2.4" dy="2.4" layer="1"/>
<text x="-2.159" y="2.159" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.159" y="-3.429" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-2.794" y1="-1.0922" x2="-2.2606" y2="1.0922" layer="51"/>
<rectangle x1="2.2606" y1="-1.0922" x2="2.794" y2="1.0922" layer="51"/>
<rectangle x1="-1.35" y1="-1.9" x2="-0.8" y2="1.9" layer="51"/>
<rectangle x1="-0.762" y1="-1.905" x2="-0.254" y2="1.905" layer="21"/>
</package>
<package name="MINIMELF">
<description>&lt;b&gt;Mini Melf Diode&lt;/b&gt;</description>
<wire x1="1.3208" y1="0.7874" x2="-1.3208" y2="0.7874" width="0.1524" layer="51"/>
<wire x1="1.3208" y1="-0.7874" x2="-1.3208" y2="-0.7874" width="0.1524" layer="51"/>
<wire x1="0.762" y1="-0.889" x2="-0.762" y2="-0.889" width="0.127" layer="21"/>
<wire x1="-0.762" y1="0.889" x2="0.762" y2="0.889" width="0.127" layer="21"/>
<smd name="C" x="-1.7" y="0" dx="1.4" dy="1.8" layer="1"/>
<smd name="A" x="1.7" y="0" dx="1.4" dy="1.8" layer="1"/>
<text x="-1.651" y="1.143" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.651" y="-2.413" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.8542" y1="-0.8636" x2="-1.2954" y2="0.8636" layer="51"/>
<rectangle x1="1.2954" y1="-0.8636" x2="1.8542" y2="0.8636" layer="51"/>
<rectangle x1="-0.635" y1="-0.889" x2="-0.127" y2="0.889" layer="21"/>
</package>
<package name="PN61729-S">
<description>&lt;b&gt;USB connector&lt;/b&gt; with shield&lt;p&gt;</description>
<wire x1="-5.9" y1="5.6" x2="-5.9" y2="-10.15" width="0.254" layer="51"/>
<wire x1="-5.9" y1="-10.15" x2="5.9" y2="-10.15" width="0.254" layer="21"/>
<wire x1="5.9" y1="-10.15" x2="5.9" y2="5.6" width="0.254" layer="51"/>
<wire x1="5.9" y1="5.727" x2="-5.9" y2="5.727" width="0.254" layer="21"/>
<wire x1="-5.9" y1="-2.02" x2="-5.9" y2="-10.15" width="0.254" layer="21"/>
<wire x1="5.9" y1="1.915" x2="5.9" y2="5.727" width="0.254" layer="21"/>
<wire x1="-5.9" y1="5.727" x2="-5.9" y2="1.915" width="0.254" layer="21"/>
<wire x1="5.9" y1="-10.15" x2="5.9" y2="-2.02" width="0.254" layer="21"/>
<pad name="1" x="1.25" y="4.71" drill="0.95" shape="octagon"/>
<pad name="2" x="-1.25" y="4.71" drill="0.95" shape="octagon"/>
<pad name="3" x="-1.25" y="2.71" drill="0.95" shape="octagon"/>
<pad name="4" x="1.25" y="2.71" drill="0.95" shape="octagon"/>
<pad name="S1" x="-6.02" y="0" drill="2.3" diameter="2.9"/>
<pad name="S2" x="6.02" y="0" drill="2.3" diameter="2.9"/>
<text x="-6.35" y="6.35" size="1.27" layer="25">&gt;NAME</text>
<text x="7.62" y="-8.89" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
</package>
<package name="HDR_2X3">
<description>&lt;b&gt;6 pin (2x3) header&lt;/b&gt;&lt;p&gt;

2x3 pin header (100mil / 2.54mm pin-spacing).

&lt;p&gt;Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<wire x1="-0.635" y1="3.81" x2="0.635" y2="3.81" width="0.2032" layer="21"/>
<wire x1="0.635" y1="3.81" x2="1.27" y2="3.175" width="0.2032" layer="21"/>
<wire x1="1.27" y1="3.175" x2="1.905" y2="3.81" width="0.2032" layer="21"/>
<wire x1="1.905" y1="3.81" x2="3.175" y2="3.81" width="0.2032" layer="21"/>
<wire x1="3.175" y1="3.81" x2="3.81" y2="3.175" width="0.2032" layer="21"/>
<wire x1="-0.635" y1="3.81" x2="-1.27" y2="3.175" width="0.2032" layer="21"/>
<wire x1="3.81" y1="3.175" x2="4.445" y2="3.81" width="0.2032" layer="21"/>
<wire x1="4.445" y1="3.81" x2="5.715" y2="3.81" width="0.2032" layer="21"/>
<wire x1="5.715" y1="3.81" x2="6.35" y2="3.175" width="0.2032" layer="21"/>
<wire x1="1.27" y1="-0.635" x2="0.635" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="3.81" y1="-0.635" x2="3.175" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="1.905" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="1.27" y2="-0.635" width="0.2032" layer="21"/>
<wire x1="-1.27" y1="3.175" x2="-1.27" y2="-0.635" width="0.2032" layer="21"/>
<wire x1="-1.27" y1="-0.635" x2="-0.635" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="-0.635" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="6.35" y1="-0.635" x2="5.715" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="5.715" y1="-1.27" x2="4.445" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="4.445" y1="-1.27" x2="3.81" y2="-0.635" width="0.2032" layer="21"/>
<wire x1="6.35" y1="3.175" x2="6.35" y2="-0.635" width="0.2032" layer="21"/>
<pad name="1" x="0" y="0" drill="0.9144" diameter="1.524" shape="square"/>
<pad name="3" x="2.54" y="0" drill="0.9144" diameter="1.524"/>
<pad name="5" x="5.08" y="0" drill="0.9144" diameter="1.524"/>
<pad name="2" x="0" y="2.54" drill="0.9144" diameter="1.524"/>
<pad name="4" x="2.54" y="2.54" drill="0.9144" diameter="1.524"/>
<pad name="6" x="5.08" y="2.54" drill="0.9144" diameter="1.524"/>
<text x="-0.381" y="-0.635" size="1.27" layer="51" font="vector" ratio="10">1</text>
<text x="-1.27" y="4.191" size="1.27" layer="25" font="vector" ratio="16">&gt;NAME</text>
<text x="-1.27" y="-2.921" size="1.27" layer="27" font="vector" ratio="16">&gt;VALUE</text>
<text x="-0.508" y="1.905" size="1.27" layer="51" font="vector" ratio="10">2</text>
<text x="0.889" y="0.889" size="0.8128" layer="51" font="vector" ratio="10">&gt;NAME</text>
</package>
<package name="HDR_S_2X3">
<description>&lt;b&gt;6 pin (2x3) shrouded IDC header (HARTING)&lt;/b&gt;&lt;p&gt;

Footprint based on con-harting-ml.lbr.&lt;p&gt;

Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<wire x1="-6.35" y1="3.175" x2="6.35" y2="3.175" width="0.1524" layer="21"/>
<wire x1="6.35" y1="-3.175" x2="6.35" y2="3.175" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="3.175" x2="-6.35" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="4.445" x2="-6.35" y2="4.445" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-4.445" x2="7.62" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="4.445" x2="-7.62" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-2.413" x2="2.032" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-2.413" x2="-2.032" y2="-2.413" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-3.175" x2="-2.032" y2="-2.413" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-3.175" x2="-6.35" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-3.175" x2="-2.032" y2="-3.429" width="0.1524" layer="21"/>
<wire x1="6.35" y1="4.445" x2="6.35" y2="4.699" width="0.1524" layer="21"/>
<wire x1="6.35" y1="4.699" x2="5.08" y2="4.699" width="0.1524" layer="21"/>
<wire x1="5.08" y1="4.445" x2="5.08" y2="4.699" width="0.1524" layer="21"/>
<wire x1="6.35" y1="4.445" x2="7.62" y2="4.445" width="0.1524" layer="21"/>
<wire x1="0.635" y1="4.699" x2="-0.635" y2="4.699" width="0.1524" layer="21"/>
<wire x1="0.635" y1="4.699" x2="0.635" y2="4.445" width="0.1524" layer="21"/>
<wire x1="0.635" y1="4.445" x2="5.08" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="4.699" x2="-0.635" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="4.699" x2="-6.35" y2="4.699" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="4.699" x2="-6.35" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="4.699" x2="-5.08" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="4.445" x2="-0.635" y2="4.445" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-4.445" x2="2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-4.445" x2="-2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="6.35" y1="-3.175" x2="2.032" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-3.175" x2="2.032" y2="-3.429" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-3.429" x2="2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-3.429" x2="6.604" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="6.604" y1="-3.429" x2="6.604" y2="3.429" width="0.0508" layer="21"/>
<wire x1="6.604" y1="3.429" x2="-6.604" y2="3.429" width="0.0508" layer="21"/>
<wire x1="-6.604" y1="3.429" x2="-6.604" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="-6.604" y1="-3.429" x2="-2.032" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="-2.032" y1="-3.429" x2="-2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-4.445" x2="-7.62" y2="-4.445" width="0.1524" layer="21"/>
<pad name="1" x="-2.54" y="-1.27" drill="0.9144" diameter="1.524" shape="square"/>
<pad name="2" x="-2.54" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="3" x="0" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="4" x="0" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="5" x="2.54" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="6" x="2.54" y="1.27" drill="0.9144" diameter="1.524"/>
<text x="-7.62" y="5.08" size="1.778" layer="25" font="vector" ratio="10">&gt;NAME</text>
<text x="-7.62" y="-6.604" size="1.778" layer="27" font="vector" ratio="10">&gt;VALUE</text>
<text x="-5.08" y="-1.905" size="1.27" layer="21" font="vector" ratio="10">1</text>
<text x="-5.08" y="0.635" size="1.27" layer="21" font="vector" ratio="10">2</text>
<text x="-0.381" y="-4.064" size="1.27" layer="21" font="vector" ratio="10">6</text>
<text x="3.81" y="0.635" size="1.27" layer="21" font="vector">6</text>
<text x="3.81" y="-1.905" size="1.27" layer="21" font="vector">5</text>
</package>
<package name="TP-TH-15-06">
<description>&lt;h1&gt;Test pad Ø1.5mm Thru-hole 0.6mm drill&lt;/h1&gt;&lt;p&gt;
With name and circle in silk-screen.&lt;p&gt;
Drawn by Mikkel Holm Olsen &lt;i&gt;&amp;lt;eagle@symlink.dk&amp;gt;&lt;/i&gt;&lt;p&gt;</description>
<circle x="0" y="0" radius="0.9398" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="0.8131" width="0.0508" layer="51"/>
<circle x="0" y="0" radius="0.8131" width="0.0508" layer="52"/>
<circle x="0" y="0" radius="0.9398" width="0.127" layer="22"/>
<pad name="1" x="0" y="0" drill="0.6" diameter="1.5"/>
<text x="1.143" y="-0.508" size="1.016" layer="25" font="vector" ratio="15">&gt;NAME</text>
<text x="-0.762" y="-0.127" size="0.3556" layer="51">&gt;NAME</text>
<text x="0.762" y="-0.127" size="0.3556" layer="52" rot="MR0">&gt;NAME</text>
</package>
<package name="HOLE_PAD_3.2">
<pad name="P$0" x="0" y="0" drill="3.2" diameter="6.5" thermals="no"/>
</package>
</packages>
<symbols>
<symbol name="DIN5_FEMALE">
<wire x1="10.16" y1="5.08" x2="0.762" y2="5.08" width="0.1524" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="0.762" y2="-5.08" width="0.1524" layer="94"/>
<wire x1="10.16" y1="0" x2="5.842" y2="0" width="0.1524" layer="94"/>
<wire x1="10.16" y1="2.54" x2="4.572" y2="2.54" width="0.1524" layer="94"/>
<wire x1="4.572" y1="2.54" x2="4.064" y2="3.048" width="0.1524" layer="94"/>
<wire x1="10.16" y1="-2.54" x2="4.572" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="4.572" y1="-2.54" x2="4.064" y2="-3.048" width="0.1524" layer="94"/>
<wire x1="-10.16" y1="-10.16" x2="-10.16" y2="10.16" width="0.254" layer="94"/>
<wire x1="-10.16" y1="10.16" x2="10.16" y2="10.16" width="0.254" layer="94"/>
<wire x1="10.16" y1="10.16" x2="10.16" y2="-10.16" width="0.254" layer="94"/>
<wire x1="10.16" y1="-10.16" x2="-10.16" y2="-10.16" width="0.254" layer="94"/>
<wire x1="6.604" y1="-6.096" x2="-6.35" y2="-6.35" width="0.254" layer="94" style="shortdash" curve="-94.999779"/>
<wire x1="12.7" y1="-17.78" x2="12.7" y2="-10.16" width="0.1524" layer="94"/>
<wire x1="12.7" y1="-10.16" x2="6.35" y2="-6.604" width="0.1524" layer="94"/>
<circle x="0" y="0" radius="7.62" width="0.8128" layer="94"/>
<circle x="3.556" y="-3.556" radius="0.635" width="0.254" layer="94"/>
<circle x="0" y="5.08" radius="0.635" width="0.254" layer="94"/>
<circle x="0" y="-5.08" radius="0.635" width="0.254" layer="94"/>
<circle x="5.08" y="0" radius="0.635" width="0.254" layer="94"/>
<circle x="3.556" y="3.556" radius="0.635" width="0.254" layer="94"/>
<text x="-10.16" y="11.049" size="1.778" layer="96">&gt;VALUE</text>
<text x="-10.16" y="13.589" size="1.778" layer="95">&gt;NAME</text>
<text x="11.43" y="5.334" size="1.524" layer="94">1</text>
<text x="11.43" y="2.794" size="1.524" layer="94">4</text>
<text x="11.43" y="0.254" size="1.524" layer="94">2</text>
<text x="11.43" y="-2.286" size="1.524" layer="94">5</text>
<text x="11.43" y="-4.826" size="1.524" layer="94">3</text>
<rectangle x1="-7.62" y1="-1.27" x2="-5.08" y2="1.27" layer="94"/>
<pin name="1" x="15.24" y="5.08" visible="off" length="middle" direction="pas" swaplevel="2" rot="R180"/>
<pin name="2" x="15.24" y="0" visible="off" length="middle" direction="pas" swaplevel="2" rot="R180"/>
<pin name="3" x="15.24" y="-5.08" visible="off" length="middle" direction="pas" swaplevel="2" rot="R180"/>
<pin name="4" x="15.24" y="2.54" visible="off" length="middle" direction="pas" swaplevel="2" rot="R180"/>
<pin name="5" x="15.24" y="-2.54" visible="off" length="middle" direction="pas" swaplevel="2" rot="R180"/>
<pin name="PE@1" x="15.24" y="-10.16" visible="off" length="short" direction="pwr" swaplevel="1" rot="R180"/>
<pin name="PE@2" x="15.24" y="-12.7" visible="off" length="short" direction="pwr" swaplevel="1" rot="R180"/>
<pin name="PE@3" x="15.24" y="-15.24" visible="off" length="short" direction="pwr" swaplevel="1" rot="R180"/>
<pin name="PE@4" x="15.24" y="-17.78" visible="off" length="short" direction="pwr" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="USB-SHIELD">
<wire x1="0" y1="7.62" x2="0" y2="-5.08" width="0.254" layer="94"/>
<wire x1="0" y1="-5.08" x2="5.08" y2="-5.08" width="0.254" layer="94"/>
<wire x1="0" y1="7.62" x2="5.08" y2="7.62" width="0.254" layer="94"/>
<wire x1="2.54" y1="-10.16" x2="2.54" y2="-9.398" width="0.1524" layer="94"/>
<wire x1="2.54" y1="-8.89" x2="2.54" y2="-8.128" width="0.1524" layer="94"/>
<wire x1="6.35" y1="-5.334" x2="6.35" y2="-4.572" width="0.1524" layer="94"/>
<wire x1="6.35" y1="-4.064" x2="6.35" y2="-3.302" width="0.1524" layer="94"/>
<wire x1="6.35" y1="-2.794" x2="6.35" y2="-2.032" width="0.1524" layer="94"/>
<wire x1="6.35" y1="-1.524" x2="6.35" y2="-0.762" width="0.1524" layer="94"/>
<wire x1="6.35" y1="-0.254" x2="6.35" y2="0.508" width="0.1524" layer="94"/>
<wire x1="6.35" y1="1.016" x2="6.35" y2="1.778" width="0.1524" layer="94"/>
<wire x1="6.35" y1="2.286" x2="6.35" y2="3.048" width="0.1524" layer="94"/>
<wire x1="6.35" y1="3.556" x2="6.35" y2="4.318" width="0.1524" layer="94"/>
<wire x1="6.35" y1="4.826" x2="6.35" y2="5.588" width="0.1524" layer="94"/>
<wire x1="6.35" y1="6.096" x2="6.35" y2="6.858" width="0.1524" layer="94"/>
<wire x1="6.35" y1="7.366" x2="6.35" y2="8.128" width="0.1524" layer="94"/>
<wire x1="6.35" y1="8.382" x2="5.588" y2="8.382" width="0.1524" layer="94"/>
<wire x1="5.08" y1="8.382" x2="4.318" y2="8.382" width="0.1524" layer="94"/>
<wire x1="3.81" y1="8.382" x2="3.048" y2="8.382" width="0.1524" layer="94"/>
<wire x1="2.54" y1="8.382" x2="1.778" y2="8.382" width="0.1524" layer="94"/>
<wire x1="2.54" y1="-7.112" x2="2.54" y2="-6.35" width="0.1524" layer="94"/>
<wire x1="3.556" y1="-5.842" x2="2.794" y2="-5.842" width="0.1524" layer="94"/>
<wire x1="4.826" y1="-5.842" x2="4.064" y2="-5.842" width="0.1524" layer="94"/>
<wire x1="6.096" y1="-5.842" x2="5.334" y2="-5.842" width="0.1524" layer="94"/>
<text x="0" y="8.89" size="1.778" layer="95">&gt;NAME</text>
<text x="0" y="-12.7" size="1.778" layer="96">&gt;VALUE</text>
<text x="5.08" y="-2.54" size="2.54" layer="94" rot="R90">USB</text>
<pin name="1" x="-2.54" y="5.08" visible="pad" length="short" direction="pas"/>
<pin name="2" x="-2.54" y="2.54" visible="pad" length="short" direction="pas"/>
<pin name="3" x="-2.54" y="0" visible="pad" length="short" direction="pas"/>
<pin name="4" x="-2.54" y="-2.54" visible="pad" length="short" direction="pas"/>
<pin name="S1" x="0" y="-7.62" visible="off" length="short" direction="pas"/>
<pin name="S2" x="0" y="-10.16" visible="off" length="short" direction="pas"/>
</symbol>
<symbol name="R-EU_">
<wire x1="-2.54" y1="-0.889" x2="2.54" y2="-0.889" width="0.254" layer="94"/>
<wire x1="2.54" y1="0.889" x2="-2.54" y2="0.889" width="0.254" layer="94"/>
<wire x1="2.54" y1="-0.889" x2="2.54" y2="0.889" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-0.889" x2="-2.54" y2="0.889" width="0.254" layer="94"/>
<text x="-3.81" y="1.4986" size="1.778" layer="95">&gt;NAME</text>
<text x="-3.81" y="-3.302" size="1.778" layer="96">&gt;VALUE</text>
<pin name="2" x="5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1" rot="R180"/>
<pin name="1" x="-5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1"/>
</symbol>
<symbol name="138">
<wire x1="3.81" y1="1.27" x2="3.81" y2="5.08" width="0.1524" layer="94"/>
<wire x1="5.08" y1="-2.54" x2="6.35" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="5.08" y1="-2.54" x2="5.08" y2="2.54" width="0.1524" layer="94"/>
<wire x1="-5.715" y1="2.54" x2="-6.985" y2="0" width="0.254" layer="94"/>
<wire x1="-6.985" y1="0" x2="-8.255" y2="2.54" width="0.254" layer="94"/>
<wire x1="-5.715" y1="0" x2="-6.985" y2="0" width="0.254" layer="94"/>
<wire x1="-6.985" y1="0" x2="-8.255" y2="0" width="0.254" layer="94"/>
<wire x1="-5.08" y1="1.905" x2="-3.81" y2="1.905" width="0.1524" layer="94"/>
<wire x1="-3.81" y1="1.905" x2="-4.191" y2="2.159" width="0.1524" layer="94"/>
<wire x1="-3.81" y1="1.905" x2="-4.191" y2="1.651" width="0.1524" layer="94"/>
<wire x1="-5.08" y1="0.635" x2="-3.81" y2="0.635" width="0.1524" layer="94"/>
<wire x1="-3.81" y1="0.635" x2="-4.191" y2="0.889" width="0.1524" layer="94"/>
<wire x1="-3.81" y1="0.635" x2="-4.191" y2="0.381" width="0.1524" layer="94"/>
<wire x1="-5.715" y1="2.54" x2="-6.985" y2="2.54" width="0.254" layer="94"/>
<wire x1="-6.985" y1="2.54" x2="-6.985" y2="0" width="0.254" layer="94"/>
<wire x1="-6.985" y1="2.54" x2="-8.255" y2="2.54" width="0.254" layer="94"/>
<wire x1="-2.54" y1="0" x2="-1.27" y2="2.54" width="0.254" layer="94"/>
<wire x1="-1.27" y1="2.54" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="-2.54" y1="2.54" x2="-1.27" y2="2.54" width="0.254" layer="94"/>
<wire x1="-1.27" y1="2.54" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="-2.54" y1="0" x2="-1.27" y2="0" width="0.254" layer="94"/>
<wire x1="-1.27" y1="-1.27" x2="-1.27" y2="0" width="0.1524" layer="94"/>
<wire x1="-1.27" y1="0" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="-1.27" y1="0" x2="-1.27" y2="2.54" width="0.254" layer="94"/>
<wire x1="3.81" y1="5.08" x2="-1.27" y2="5.08" width="0.1524" layer="94"/>
<wire x1="-1.27" y1="5.08" x2="-1.27" y2="2.54" width="0.1524" layer="94"/>
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="-1.27" width="0.1524" layer="94"/>
<wire x1="5.08" y1="-3.81" x2="5.08" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-6.985" y1="2.54" x2="-6.985" y2="5.08" width="0.1524" layer="94"/>
<wire x1="5.08" y1="2.54" x2="10.16" y2="2.54" width="0.1524" layer="94"/>
<wire x1="3.81" y1="5.08" x2="10.16" y2="5.08" width="0.1524" layer="94"/>
<wire x1="9.525" y1="7.62" x2="-9.525" y2="7.62" width="0.4064" layer="94"/>
<wire x1="-10.16" y1="5.08" x2="-6.985" y2="5.08" width="0.1524" layer="94"/>
<wire x1="9.525" y1="-7.62" x2="-9.525" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="-10.16" y1="-5.08" x2="-6.985" y2="-5.08" width="0.1524" layer="94"/>
<wire x1="-6.985" y1="-5.08" x2="-6.985" y2="0" width="0.1524" layer="94"/>
<wire x1="-9.525" y1="7.62" x2="-9.525" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="9.525" y1="7.62" x2="9.525" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="3.81" y1="1.27" x2="1.27" y2="-1.27" width="0.1524" layer="94"/>
<wire x1="1.27" y1="-1.27" x2="3.556" y2="-3.556" width="0.1524" layer="94"/>
<wire x1="3.81" y1="-3.81" x2="5.08" y2="-3.81" width="0.1524" layer="94"/>
<wire x1="3.048" y1="-2.286" x2="3.556" y2="-3.556" width="0.1524" layer="94"/>
<wire x1="3.556" y1="-3.556" x2="3.81" y2="-3.81" width="0.1524" layer="94"/>
<wire x1="3.556" y1="-3.556" x2="2.286" y2="-3.048" width="0.1524" layer="94"/>
<wire x1="2.286" y1="-3.048" x2="3.048" y2="-2.286" width="0.1524" layer="94"/>
<wire x1="8.89" y1="0" x2="6.35" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="6.35" y1="-2.54" x2="8.636" y2="-4.826" width="0.1524" layer="94"/>
<wire x1="8.89" y1="-5.08" x2="10.16" y2="-5.08" width="0.1524" layer="94"/>
<wire x1="8.89" y1="0" x2="10.16" y2="0" width="0.1524" layer="94"/>
<wire x1="8.128" y1="-3.556" x2="8.636" y2="-4.826" width="0.1524" layer="94"/>
<wire x1="8.636" y1="-4.826" x2="8.89" y2="-5.08" width="0.1524" layer="94"/>
<wire x1="8.636" y1="-4.826" x2="7.366" y2="-4.318" width="0.1524" layer="94"/>
<wire x1="7.366" y1="-4.318" x2="8.128" y2="-3.556" width="0.1524" layer="94"/>
<circle x="3.81" y="5.08" radius="0.127" width="0.4064" layer="94"/>
<circle x="5.08" y="-2.54" radius="0.127" width="0.4064" layer="94"/>
<text x="-9.525" y="8.255" size="1.778" layer="95">&gt;NAME</text>
<text x="-9.525" y="-10.16" size="1.778" layer="96">&gt;VALUE</text>
<text x="-8.255" y="4.064" size="0.8128" layer="93">A</text>
<text x="-8.255" y="-4.318" size="0.8128" layer="93">C</text>
<text x="5.08" y="5.334" size="0.8128" layer="93">Vcc</text>
<text x="5.08" y="2.794" size="0.8128" layer="93">Vb</text>
<text x="7.62" y="0.381" size="0.8128" layer="93">Vo</text>
<text x="6.858" y="-6.223" size="0.8128" layer="93">GND</text>
<rectangle x1="0.889" y1="-3.81" x2="1.651" y2="1.27" layer="94"/>
<rectangle x1="5.969" y1="-5.08" x2="6.731" y2="0" layer="94"/>
<pin name="A" x="-12.7" y="5.08" visible="pad" length="short" direction="pas"/>
<pin name="C" x="-12.7" y="-5.08" visible="pad" length="short" direction="pas"/>
<pin name="VB" x="12.7" y="2.54" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="VCC" x="12.7" y="5.08" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="GND" x="12.7" y="-5.08" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="VO" x="12.7" y="0" visible="pad" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="GND">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
<symbol name="ZENER_DIODE">
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="-1.27" y2="1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="1.27" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="-1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="1.27" y2="-1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="-1.27" x2="0.635" y2="-1.27" width="0.254" layer="94"/>
<text x="-1.778" y="1.905" size="1.778" layer="95">&gt;NAME</text>
<text x="-1.778" y="-3.429" size="1.778" layer="96">&gt;VALUE</text>
<pin name="A" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
<pin name="C" x="2.54" y="0" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="R-TRIMM">
<wire x1="0.762" y1="2.54" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="-0.762" y1="2.54" x2="-0.762" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0.762" y1="-2.54" x2="0.762" y2="2.54" width="0.254" layer="94"/>
<wire x1="2.54" y1="0" x2="1.651" y2="0" width="0.1524" layer="94"/>
<wire x1="1.651" y1="0" x2="-1.8796" y2="1.7526" width="0.1524" layer="94"/>
<wire x1="0" y1="2.54" x2="0" y2="5.08" width="0.1524" layer="94"/>
<wire x1="0" y1="2.54" x2="-0.762" y2="2.54" width="0.254" layer="94"/>
<wire x1="-0.762" y1="-2.54" x2="0.762" y2="-2.54" width="0.254" layer="94"/>
<wire x1="-2.286" y1="1.27" x2="-1.651" y2="2.413" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-2.54" x2="-2.54" y2="-0.508" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="-0.508" x2="-3.048" y2="-1.524" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="-0.508" x2="-2.032" y2="-1.524" width="0.1524" layer="94"/>
<text x="-5.969" y="-3.81" size="1.778" layer="95" rot="R90">&gt;NAME</text>
<text x="-3.81" y="-3.81" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="E" x="0" y="-5.08" visible="pad" length="short" direction="pas" rot="R90"/>
<pin name="A" x="0" y="5.08" visible="pad" length="short" direction="pas" rot="R270"/>
<pin name="S" x="5.08" y="0" visible="pad" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="DIODE">
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="0" width="0.2032" layer="94"/>
<wire x1="1.27" y1="0" x2="-1.27" y2="1.27" width="0.2032" layer="94"/>
<wire x1="1.27" y1="1.27" x2="1.27" y2="0" width="0.2032" layer="94"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="0" width="0.2032" layer="94"/>
<wire x1="-1.27" y1="0" x2="-1.27" y2="-1.27" width="0.2032" layer="94"/>
<wire x1="1.27" y1="0" x2="1.27" y2="-1.27" width="0.2032" layer="94"/>
<wire x1="-1.27" y1="0" x2="-2.54" y2="0" width="0.2032" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.2032" layer="94"/>
<text x="-2.54" y="5.5626" size="1.778" layer="95" font="vector">&gt;NAME</text>
<text x="-2.54" y="2.7686" size="1.778" layer="96" font="vector">&gt;VALUE</text>
<pin name="A" x="-2.54" y="0" visible="off" length="point" direction="pas"/>
<pin name="C" x="2.54" y="0" visible="off" length="point" direction="pas" rot="R180"/>
</symbol>
<symbol name="SUPPRESSOR">
<wire x1="1.27" y1="-1.27" x2="0" y2="1.27" width="0.254" layer="94"/>
<wire x1="0" y1="1.27" x2="-1.27" y2="-1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="1.905" x2="1.27" y2="1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="1.27" x2="0" y2="1.27" width="0.254" layer="94"/>
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="-1.27" width="0.254" layer="94"/>
<wire x1="0" y1="1.27" x2="-1.27" y2="1.27" width="0.254" layer="94"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="0.635" width="0.254" layer="94"/>
<text x="2.794" y="1.905" size="1.778" layer="95">&gt;NAME</text>
<text x="2.794" y="-0.889" size="1.778" layer="96">&gt;VALUE</text>
<pin name="A" x="0" y="-2.54" visible="off" length="short" direction="pas" rot="R90"/>
<pin name="C" x="0" y="2.54" visible="off" length="short" direction="pas" rot="R270"/>
</symbol>
<symbol name="MA2X3">
<wire x1="-3.81" y1="-2.54" x2="-11.43" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="5.08" x2="-5.08" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="2.54" x2="-5.08" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="0" x2="-5.08" y2="0" width="0.6096" layer="94"/>
<wire x1="-10.16" y1="5.08" x2="-8.89" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-10.16" y1="2.54" x2="-8.89" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-10.16" y1="0" x2="-8.89" y2="0" width="0.6096" layer="94"/>
<wire x1="-11.43" y1="7.62" x2="-11.43" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="-2.54" x2="-3.81" y2="7.62" width="0.4064" layer="94"/>
<wire x1="-11.43" y1="7.62" x2="-3.81" y2="7.62" width="0.4064" layer="94"/>
<text x="-11.43" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<text x="-11.43" y="8.382" size="1.778" layer="95">&gt;NAME</text>
<text x="-3.048" y="0.254" size="1.27" layer="95">1</text>
<text x="-13.208" y="0.254" size="1.27" layer="95">2</text>
<text x="-3.048" y="2.794" size="1.27" layer="95">3</text>
<text x="-13.208" y="2.794" size="1.27" layer="95">4</text>
<text x="-3.048" y="5.334" size="1.27" layer="95">5</text>
<text x="-13.208" y="5.334" size="1.27" layer="95">6</text>
<pin name="1" x="0" y="0" visible="off" length="middle" direction="pas" rot="R180"/>
<pin name="3" x="0" y="2.54" visible="off" length="middle" direction="pas" rot="R180"/>
<pin name="5" x="0" y="5.08" visible="off" length="middle" direction="pas" rot="R180"/>
<pin name="2" x="-15.24" y="0" visible="off" length="middle" direction="pas"/>
<pin name="4" x="-15.24" y="2.54" visible="off" length="middle" direction="pas"/>
<pin name="6" x="-15.24" y="5.08" visible="off" length="middle" direction="pas"/>
</symbol>
<symbol name="TP">
<wire x1="-0.762" y1="-0.762" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="0.762" y2="-0.762" width="0.254" layer="94"/>
<wire x1="0.762" y1="-0.762" x2="0" y2="-1.524" width="0.254" layer="94"/>
<wire x1="0" y1="-1.524" x2="-0.762" y2="-0.762" width="0.254" layer="94"/>
<text x="-1.524" y="0.508" size="1.778" layer="95">&gt;NAME</text>
<pin name="TP" x="0" y="-2.54" visible="off" length="short" direction="in" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="6N138" prefix="OK">
<description>6N138 &lt;b&gt;OPTOCOUPLER&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="138" x="0" y="0"/>
</gates>
<devices>
<device name="DIL08" package="DIL08">
<connects>
<connect gate="G$1" pin="A" pad="2"/>
<connect gate="G$1" pin="C" pad="3"/>
<connect gate="G$1" pin="GND" pad="5"/>
<connect gate="G$1" pin="VB" pad="7"/>
<connect gate="G$1" pin="VCC" pad="8"/>
<connect gate="G$1" pin="VO" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="GND" prefix="GND">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="R-TRIMM" prefix="R" uservalue="yes">
<description>&lt;B&gt;TRIMM RESISTOR&lt;/B&gt;</description>
<gates>
<gate name="G$1" symbol="R-TRIMM" x="0" y="0"/>
</gates>
<devices>
<device name="" package="RTRIM3339P">
<connects>
<connect gate="G$1" pin="A" pad="3"/>
<connect gate="G$1" pin="E" pad="1"/>
<connect gate="G$1" pin="S" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="R-EU" prefix="R" uservalue="yes">
<description>Resistor european symbol &lt;b&gt;RESISTOR&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="R-EU_" x="0" y="0"/>
</gates>
<devices>
<device name="0805" package="R0805">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="LEADED-7.5" package="0207/7">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="DIN5_FEMALE_PCB" prefix="X">
<description>PCB Mount Female &lt;b&gt;CONNECTOR&lt;/b&gt;&lt;p&gt;
DIN 5 pins 90 deg. with shield, horizontal (DIN 41524)</description>
<gates>
<gate name="G$1" symbol="DIN5_FEMALE" x="0" y="0"/>
</gates>
<devices>
<device name="" package="DIN5_FEM_UNIV">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="PE@1" pad="PE@1"/>
<connect gate="G$1" pin="PE@2" pad="PE@2"/>
<connect gate="G$1" pin="PE@3" pad="PE@3"/>
<connect gate="G$1" pin="PE@4" pad="PE@4"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="ZENER-DIODE" prefix="D" uservalue="yes">
<description>Z-Diode</description>
<gates>
<gate name="G$1" symbol="ZENER_DIODE" x="0" y="0"/>
</gates>
<devices>
<device name="DO34-7" package="DO34Z7">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="SUPPRESSOR-" prefix="D" uservalue="yes">
<description>&lt;b&gt;Suppressor diode&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="SUPPRESSOR" x="0" y="0"/>
</gates>
<devices>
<device name="SMB" package="SMB">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="1N4148" prefix="D">
<description>&lt;B&gt;DIODE&lt;/B&gt;&lt;p&gt;
high speed (Philips)</description>
<gates>
<gate name="G$1" symbol="DIODE" x="0" y="0"/>
</gates>
<devices>
<device name="MINIMELF" package="MINIMELF">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="PN61729-S" prefix="X">
<description>&lt;b&gt;BERG&lt;/b&gt; USB connector</description>
<gates>
<gate name="G$1" symbol="USB-SHIELD" x="0" y="0"/>
</gates>
<devices>
<device name="" package="PN61729-S">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="S1" pad="S1"/>
<connect gate="G$1" pin="S2" pad="S2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="HEADER2X3" prefix="CON">
<description>&lt;b&gt;6 pin (2x3) header&lt;/b&gt;&lt;p&gt;

Two-row schematics symbol</description>
<gates>
<gate name="G$1" symbol="MA2X3" x="0" y="0"/>
</gates>
<devices>
<device name="" package="HDR_2X3">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="S" package="HDR_S_2X3">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="TP" prefix="TP">
<description>&lt;h1&gt;Test pad&lt;/h1&gt;</description>
<gates>
<gate name="G$1" symbol="TP" x="0" y="0"/>
</gates>
<devices>
<device name="TH-15-06" package="TP-TH-15-06">
<connects>
<connect gate="G$1" pin="TP" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="HOLE_PAD" prefix="HOLE">
<gates>
<gate name="G$1" symbol="TP" x="0" y="2.54"/>
</gates>
<devices>
<device name="" package="HOLE_PAD_3.2">
<connects>
<connect gate="G$1" pin="TP" pad="P$0"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-head">
<packages>
<package name="HDR_2X8">
<description>&lt;b&gt;16 pin (2x8) header&lt;/b&gt;&lt;p&gt;

2x8 pin header (100mil / 2.54mm pin-spacing).

&lt;p&gt;Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<wire x1="-1.016" y1="3.81" x2="1.016" y2="3.81" width="0.2032" layer="21"/>
<wire x1="1.016" y1="3.81" x2="1.27" y2="3.556" width="0.2032" layer="21"/>
<wire x1="1.27" y1="-1.016" x2="1.016" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="1.27" y1="3.556" x2="1.524" y2="3.81" width="0.2032" layer="21"/>
<wire x1="1.524" y1="3.81" x2="3.556" y2="3.81" width="0.2032" layer="21"/>
<wire x1="3.556" y1="3.81" x2="3.81" y2="3.556" width="0.2032" layer="21"/>
<wire x1="3.81" y1="-1.016" x2="3.556" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="3.556" y1="-1.27" x2="1.524" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="1.524" y1="-1.27" x2="1.27" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="-1.27" y1="3.556" x2="-1.27" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="-1.016" y1="3.81" x2="-1.27" y2="3.556" width="0.2032" layer="21"/>
<wire x1="-1.27" y1="-1.016" x2="-1.016" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="1.016" y1="-1.27" x2="-1.016" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="3.81" y1="3.556" x2="4.064" y2="3.81" width="0.2032" layer="21"/>
<wire x1="4.064" y1="3.81" x2="6.096" y2="3.81" width="0.2032" layer="21"/>
<wire x1="6.096" y1="3.81" x2="6.35" y2="3.556" width="0.2032" layer="21"/>
<wire x1="6.35" y1="-1.016" x2="6.096" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="6.096" y1="-1.27" x2="4.064" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="4.064" y1="-1.27" x2="3.81" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="6.604" y1="3.81" x2="8.636" y2="3.81" width="0.2032" layer="21"/>
<wire x1="8.636" y1="3.81" x2="8.89" y2="3.556" width="0.2032" layer="21"/>
<wire x1="8.89" y1="-1.016" x2="8.636" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="6.604" y1="3.81" x2="6.35" y2="3.556" width="0.2032" layer="21"/>
<wire x1="6.35" y1="-1.016" x2="6.604" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="8.636" y1="-1.27" x2="6.604" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="9.144" y1="3.81" x2="11.176" y2="3.81" width="0.2032" layer="21"/>
<wire x1="11.176" y1="3.81" x2="11.43" y2="3.556" width="0.2032" layer="21"/>
<wire x1="11.43" y1="-1.016" x2="11.176" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="11.43" y1="3.556" x2="11.684" y2="3.81" width="0.2032" layer="21"/>
<wire x1="11.684" y1="3.81" x2="13.716" y2="3.81" width="0.2032" layer="21"/>
<wire x1="13.716" y1="3.81" x2="13.97" y2="3.556" width="0.2032" layer="21"/>
<wire x1="13.97" y1="-1.016" x2="13.716" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="13.716" y1="-1.27" x2="11.684" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="11.684" y1="-1.27" x2="11.43" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="9.144" y1="3.81" x2="8.89" y2="3.556" width="0.2032" layer="21"/>
<wire x1="8.89" y1="-1.016" x2="9.144" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="11.176" y1="-1.27" x2="9.144" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="13.97" y1="3.556" x2="14.224" y2="3.81" width="0.2032" layer="21"/>
<wire x1="14.224" y1="3.81" x2="16.256" y2="3.81" width="0.2032" layer="21"/>
<wire x1="16.256" y1="3.81" x2="16.51" y2="3.556" width="0.2032" layer="21"/>
<wire x1="16.51" y1="-1.016" x2="16.256" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="16.256" y1="-1.27" x2="14.224" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="14.224" y1="-1.27" x2="13.97" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="16.764" y1="3.81" x2="16.51" y2="3.556" width="0.2032" layer="21"/>
<wire x1="16.51" y1="-1.016" x2="16.764" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="19.05" y1="3.556" x2="19.05" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="18.796" y1="3.81" x2="19.05" y2="3.556" width="0.2032" layer="21"/>
<wire x1="19.05" y1="-1.016" x2="18.796" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="16.764" y1="3.81" x2="18.796" y2="3.81" width="0.2032" layer="21"/>
<wire x1="18.796" y1="-1.27" x2="16.764" y2="-1.27" width="0.2032" layer="21"/>
<pad name="1" x="0" y="0" drill="1.016" diameter="1.778" shape="square"/>
<pad name="3" x="2.54" y="0" drill="1.016" diameter="1.778"/>
<pad name="5" x="5.08" y="0" drill="1.016" diameter="1.778"/>
<pad name="7" x="7.62" y="0" drill="1.016" diameter="1.778"/>
<pad name="9" x="10.16" y="0" drill="1.016" diameter="1.778"/>
<pad name="2" x="0" y="2.54" drill="1.016" diameter="1.778"/>
<pad name="4" x="2.54" y="2.54" drill="1.016" diameter="1.778"/>
<pad name="6" x="5.08" y="2.54" drill="1.016" diameter="1.778"/>
<pad name="8" x="7.62" y="2.54" drill="1.016" diameter="1.778"/>
<pad name="10" x="10.16" y="2.54" drill="1.016" diameter="1.778"/>
<pad name="11" x="12.7" y="0" drill="1.016" diameter="1.778"/>
<pad name="12" x="12.7" y="2.54" drill="1.016" diameter="1.778"/>
<pad name="13" x="15.24" y="0" drill="1.016" diameter="1.778"/>
<pad name="14" x="15.24" y="2.54" drill="1.016" diameter="1.778"/>
<pad name="15" x="17.78" y="0" drill="1.016" diameter="1.778"/>
<pad name="16" x="17.78" y="2.54" drill="1.016" diameter="1.778"/>
<text x="-2.413" y="-0.635" size="1.27" layer="51" font="vector" ratio="10">1</text>
<text x="-1.27" y="4.191" size="1.27" layer="25" font="vector" ratio="10">&gt;NAME</text>
<text x="-1.27" y="-2.921" size="1.27" layer="27" font="vector" ratio="10">&gt;VALUE</text>
<text x="19.304" y="1.905" size="1.27" layer="51" font="vector" ratio="10">16</text>
<text x="19.304" y="-0.635" size="1.27" layer="51" font="vector" ratio="10">15</text>
<text x="-2.413" y="1.905" size="1.27" layer="51" font="vector" ratio="10">2</text>
</package>
<package name="HDR_S_2X8">
<description>&lt;b&gt;16 pin (2x8) shrouded IDC header (HARTING)&lt;/b&gt;&lt;p&gt;

Footprint based on con-harting-ml.lbr.&lt;p&gt;

Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<wire x1="-12.7" y1="3.175" x2="12.7" y2="3.175" width="0.1524" layer="21"/>
<wire x1="12.7" y1="-3.175" x2="12.7" y2="3.175" width="0.1524" layer="21"/>
<wire x1="-12.7" y1="3.175" x2="-12.7" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-13.97" y1="4.445" x2="-12.7" y2="4.445" width="0.1524" layer="21"/>
<wire x1="13.97" y1="-4.445" x2="9.271" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="13.97" y1="-4.445" x2="13.97" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-13.97" y1="4.445" x2="-13.97" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="12.7" y1="-3.175" x2="8.382" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-2.413" x2="-2.032" y2="-2.413" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-3.175" x2="-2.032" y2="-2.413" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-3.175" x2="-4.318" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-2.413" x2="2.032" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-3.175" x2="2.032" y2="-3.429" width="0.1524" layer="21"/>
<wire x1="12.7" y1="4.445" x2="12.7" y2="4.699" width="0.1524" layer="21"/>
<wire x1="12.7" y1="4.699" x2="11.43" y2="4.699" width="0.1524" layer="21"/>
<wire x1="11.43" y1="4.445" x2="11.43" y2="4.699" width="0.1524" layer="21"/>
<wire x1="12.7" y1="4.445" x2="13.97" y2="4.445" width="0.1524" layer="21"/>
<wire x1="0.635" y1="4.699" x2="-0.635" y2="4.699" width="0.1524" layer="21"/>
<wire x1="0.635" y1="4.699" x2="0.635" y2="4.445" width="0.1524" layer="21"/>
<wire x1="0.635" y1="4.445" x2="11.43" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="4.699" x2="-0.635" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-11.43" y1="4.699" x2="-12.7" y2="4.699" width="0.1524" layer="21"/>
<wire x1="-12.7" y1="4.699" x2="-12.7" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-11.43" y1="4.699" x2="-11.43" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-11.43" y1="4.445" x2="-0.635" y2="4.445" width="0.1524" layer="21"/>
<wire x1="5.969" y1="-4.445" x2="2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-4.445" x2="-2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="6.858" y1="-3.175" x2="6.858" y2="-3.429" width="0.1524" layer="21"/>
<wire x1="6.858" y1="-3.175" x2="2.032" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="8.382" y1="-3.175" x2="8.382" y2="-3.429" width="0.1524" layer="21"/>
<wire x1="8.382" y1="-3.175" x2="6.858" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="5.969" y1="-4.445" x2="6.35" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="8.89" y1="-3.937" x2="9.271" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="8.89" y1="-3.937" x2="8.382" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="6.858" y1="-3.429" x2="2.032" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="2.032" y1="-3.429" x2="2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="8.382" y1="-3.429" x2="12.954" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="12.954" y1="-3.429" x2="12.954" y2="3.429" width="0.0508" layer="21"/>
<wire x1="12.954" y1="3.429" x2="-12.954" y2="3.429" width="0.0508" layer="21"/>
<wire x1="-12.954" y1="3.429" x2="-12.954" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="-12.954" y1="-3.429" x2="-5.842" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="-2.032" y1="-3.175" x2="-2.032" y2="-3.429" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-3.429" x2="-2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="6.858" y1="-3.429" x2="6.858" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="6.858" y1="-3.937" x2="6.35" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="8.382" y1="-3.429" x2="8.382" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="8.382" y1="-3.937" x2="6.858" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="-13.97" y1="-4.445" x2="-9.652" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="-9.652" y1="-4.318" x2="-9.652" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="-9.652" y1="-4.318" x2="-8.128" y2="-4.318" width="0.1524" layer="21"/>
<wire x1="-8.128" y1="-4.445" x2="-8.128" y2="-4.318" width="0.1524" layer="21"/>
<wire x1="-8.128" y1="-4.445" x2="-6.731" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="-5.842" y1="-3.429" x2="-5.842" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="-4.318" y1="-3.429" x2="-4.318" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="-4.318" y1="-3.429" x2="-2.032" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="-5.842" y1="-3.175" x2="-5.842" y2="-3.429" width="0.1524" layer="21"/>
<wire x1="-5.842" y1="-3.175" x2="-12.7" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-4.318" y1="-3.175" x2="-4.318" y2="-3.429" width="0.1524" layer="21"/>
<wire x1="-4.318" y1="-3.175" x2="-5.842" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-4.318" y1="-3.937" x2="-5.842" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="-5.842" y1="-3.937" x2="-6.35" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="-6.731" y1="-4.445" x2="-6.35" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-3.937" x2="-3.429" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="-3.429" y1="-4.445" x2="-2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-3.937" x2="-4.318" y2="-3.937" width="0.1524" layer="21"/>
<pad name="1" x="-8.89" y="-1.27" drill="0.9144" diameter="1.524" shape="square"/>
<pad name="2" x="-8.89" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="3" x="-6.35" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="4" x="-6.35" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="5" x="-3.81" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="6" x="-3.81" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="7" x="-1.27" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="8" x="-1.27" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="9" x="1.27" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="10" x="1.27" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="11" x="3.81" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="12" x="3.81" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="13" x="6.35" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="14" x="6.35" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="15" x="8.89" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="16" x="8.89" y="1.27" drill="0.9144" diameter="1.524"/>
<text x="-13.97" y="5.08" size="1.778" layer="25" font="vector" ratio="10">&gt;NAME</text>
<text x="-13.97" y="-6.731" size="1.778" layer="27" font="vector" ratio="10">&gt;VALUE</text>
<text x="-1.016" y="-4.064" size="1.27" layer="21" font="vector" ratio="10">16</text>
<text x="-11.43" y="-1.905" size="1.27" layer="21" font="vector" ratio="10">1</text>
<text x="-11.43" y="0.635" size="1.27" layer="21" font="vector" ratio="10">2</text>
<text x="10.033" y="0.635" size="1.27" layer="21" font="vector">16</text>
<text x="10.033" y="-1.905" size="1.27" layer="21" font="vector">15</text>
</package>
<package name="HDR_2X3">
<description>&lt;b&gt;6 pin (2x3) header&lt;/b&gt;&lt;p&gt;

2x3 pin header (100mil / 2.54mm pin-spacing).

&lt;p&gt;Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<wire x1="-0.635" y1="3.81" x2="0.635" y2="3.81" width="0.2032" layer="21"/>
<wire x1="0.635" y1="3.81" x2="1.27" y2="3.175" width="0.2032" layer="21"/>
<wire x1="1.27" y1="3.175" x2="1.905" y2="3.81" width="0.2032" layer="21"/>
<wire x1="1.905" y1="3.81" x2="3.175" y2="3.81" width="0.2032" layer="21"/>
<wire x1="3.175" y1="3.81" x2="3.81" y2="3.175" width="0.2032" layer="21"/>
<wire x1="-0.635" y1="3.81" x2="-1.27" y2="3.175" width="0.2032" layer="21"/>
<wire x1="3.81" y1="3.175" x2="4.445" y2="3.81" width="0.2032" layer="21"/>
<wire x1="4.445" y1="3.81" x2="5.715" y2="3.81" width="0.2032" layer="21"/>
<wire x1="5.715" y1="3.81" x2="6.35" y2="3.175" width="0.2032" layer="21"/>
<wire x1="1.27" y1="-0.635" x2="0.635" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="3.81" y1="-0.635" x2="3.175" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="1.905" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="1.27" y2="-0.635" width="0.2032" layer="21"/>
<wire x1="-1.27" y1="3.175" x2="-1.27" y2="-0.635" width="0.2032" layer="21"/>
<wire x1="-1.27" y1="-0.635" x2="-0.635" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="-0.635" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="6.35" y1="-0.635" x2="5.715" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="5.715" y1="-1.27" x2="4.445" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="4.445" y1="-1.27" x2="3.81" y2="-0.635" width="0.2032" layer="21"/>
<wire x1="6.35" y1="3.175" x2="6.35" y2="-0.635" width="0.2032" layer="21"/>
<pad name="1" x="0" y="0" drill="1.016" diameter="1.778" shape="square"/>
<pad name="3" x="2.54" y="0" drill="1.016" diameter="1.778"/>
<pad name="5" x="5.08" y="0" drill="1.016" diameter="1.778"/>
<pad name="2" x="0" y="2.54" drill="1.016" diameter="1.778"/>
<pad name="4" x="2.54" y="2.54" drill="1.016" diameter="1.778"/>
<pad name="6" x="5.08" y="2.54" drill="1.016" diameter="1.778"/>
<text x="-0.381" y="-0.635" size="1.27" layer="51" font="vector" ratio="10">1</text>
<text x="-1.27" y="4.191" size="1.27" layer="25" font="vector" ratio="16">&gt;NAME</text>
<text x="-1.27" y="-2.921" size="1.27" layer="27" font="vector" ratio="16">&gt;VALUE</text>
<text x="-0.508" y="1.905" size="1.27" layer="51" font="vector" ratio="10">2</text>
<text x="0.889" y="0.889" size="0.8128" layer="51" font="vector" ratio="10">&gt;NAME</text>
</package>
<package name="HDR_S_2X3">
<description>&lt;b&gt;6 pin (2x3) shrouded IDC header (HARTING)&lt;/b&gt;&lt;p&gt;

Footprint based on con-harting-ml.lbr.&lt;p&gt;

Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<wire x1="-6.35" y1="3.175" x2="6.35" y2="3.175" width="0.1524" layer="21"/>
<wire x1="6.35" y1="-3.175" x2="6.35" y2="3.175" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="3.175" x2="-6.35" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="4.445" x2="-6.35" y2="4.445" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-4.445" x2="7.62" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="4.445" x2="-7.62" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-2.413" x2="2.032" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-2.413" x2="-2.032" y2="-2.413" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-3.175" x2="-2.032" y2="-2.413" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-3.175" x2="-6.35" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-3.175" x2="-2.032" y2="-3.429" width="0.1524" layer="21"/>
<wire x1="6.35" y1="4.445" x2="6.35" y2="4.699" width="0.1524" layer="21"/>
<wire x1="6.35" y1="4.699" x2="5.08" y2="4.699" width="0.1524" layer="21"/>
<wire x1="5.08" y1="4.445" x2="5.08" y2="4.699" width="0.1524" layer="21"/>
<wire x1="6.35" y1="4.445" x2="7.62" y2="4.445" width="0.1524" layer="21"/>
<wire x1="0.635" y1="4.699" x2="-0.635" y2="4.699" width="0.1524" layer="21"/>
<wire x1="0.635" y1="4.699" x2="0.635" y2="4.445" width="0.1524" layer="21"/>
<wire x1="0.635" y1="4.445" x2="5.08" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="4.699" x2="-0.635" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="4.699" x2="-6.35" y2="4.699" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="4.699" x2="-6.35" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="4.699" x2="-5.08" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="4.445" x2="-0.635" y2="4.445" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-4.445" x2="2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-4.445" x2="-2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="6.35" y1="-3.175" x2="2.032" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-3.175" x2="2.032" y2="-3.429" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-3.429" x2="2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-3.429" x2="6.604" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="6.604" y1="-3.429" x2="6.604" y2="3.429" width="0.0508" layer="21"/>
<wire x1="6.604" y1="3.429" x2="-6.604" y2="3.429" width="0.0508" layer="21"/>
<wire x1="-6.604" y1="3.429" x2="-6.604" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="-6.604" y1="-3.429" x2="-2.032" y2="-3.429" width="0.0508" layer="21"/>
<wire x1="-2.032" y1="-3.429" x2="-2.032" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="-4.445" x2="-7.62" y2="-4.445" width="0.1524" layer="21"/>
<pad name="1" x="-2.54" y="-1.27" drill="0.9144" diameter="1.524" shape="square"/>
<pad name="2" x="-2.54" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="3" x="0" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="4" x="0" y="1.27" drill="0.9144" diameter="1.524"/>
<pad name="5" x="2.54" y="-1.27" drill="0.9144" diameter="1.524"/>
<pad name="6" x="2.54" y="1.27" drill="0.9144" diameter="1.524"/>
<text x="-7.62" y="5.08" size="1.778" layer="25" font="vector" ratio="10">&gt;NAME</text>
<text x="-7.62" y="-6.604" size="1.778" layer="27" font="vector" ratio="10">&gt;VALUE</text>
<text x="-5.08" y="-1.905" size="1.27" layer="21" font="vector" ratio="10">1</text>
<text x="-5.08" y="0.635" size="1.27" layer="21" font="vector" ratio="10">2</text>
<text x="-0.381" y="-4.064" size="1.27" layer="21" font="vector" ratio="10">6</text>
<text x="3.81" y="0.635" size="1.27" layer="21" font="vector">6</text>
<text x="3.81" y="-1.905" size="1.27" layer="21" font="vector">5</text>
</package>
<package name="HDR_SE_2X3">
<description>&lt;b&gt;6 pin (2x3) shrouded IDC header (HARTING). Edge mounted.&lt;/b&gt;&lt;p&gt;

Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<wire x1="7.62" y1="2.54" x2="-7.62" y2="2.54" width="0.127" layer="51"/>
<wire x1="-7.62" y1="2.54" x2="-7.62" y2="11.43" width="0.127" layer="51"/>
<wire x1="-7.62" y1="11.43" x2="-3.302" y2="11.43" width="0.127" layer="51"/>
<wire x1="-3.302" y1="11.43" x2="-2.54" y2="11.43" width="0.127" layer="51"/>
<wire x1="-2.54" y1="11.43" x2="2.54" y2="11.43" width="0.127" layer="51"/>
<wire x1="2.54" y1="11.43" x2="7.62" y2="11.43" width="0.127" layer="51"/>
<wire x1="7.62" y1="11.43" x2="7.62" y2="2.54" width="0.127" layer="51"/>
<wire x1="-2.032" y1="10.922" x2="-2.032" y2="4.826" width="0.127" layer="51"/>
<wire x1="-2.032" y1="4.826" x2="-2.032" y2="3.556" width="0.127" layer="51"/>
<wire x1="-2.032" y1="3.556" x2="2.032" y2="3.556" width="0.127" layer="51"/>
<wire x1="2.032" y1="3.556" x2="2.032" y2="4.826" width="0.127" layer="51"/>
<wire x1="2.032" y1="4.826" x2="2.032" y2="10.922" width="0.127" layer="51"/>
<wire x1="2.032" y1="10.922" x2="2.54" y2="11.43" width="0.127" layer="51"/>
<wire x1="-2.032" y1="10.922" x2="-2.54" y2="11.43" width="0.127" layer="51"/>
<wire x1="-2.032" y1="4.826" x2="2.032" y2="4.826" width="0.127" layer="51"/>
<wire x1="-3.302" y1="11.43" x2="-2.54" y2="9.906" width="0.127" layer="51"/>
<wire x1="-2.54" y1="9.906" x2="-2.032" y2="10.922" width="0.127" layer="51"/>
<wire x1="-8.128" y1="2.54" x2="-9.652" y2="2.54" width="0.127" layer="51"/>
<wire x1="-9.652" y1="2.54" x2="-12.192" y2="2.54" width="0.127" layer="51"/>
<wire x1="-12.192" y1="2.54" x2="-13.716" y2="2.54" width="0.127" layer="51"/>
<wire x1="-12.954" y1="3.302" x2="-12.192" y2="2.54" width="0.127" layer="51"/>
<wire x1="-12.192" y1="2.54" x2="-11.43" y2="3.302" width="0.127" layer="51"/>
<wire x1="-10.414" y1="3.302" x2="-9.652" y2="2.54" width="0.127" layer="51"/>
<wire x1="-9.652" y1="2.54" x2="-8.89" y2="3.302" width="0.127" layer="51"/>
<wire x1="13.716" y1="2.54" x2="12.192" y2="2.54" width="0.127" layer="51"/>
<wire x1="12.192" y1="2.54" x2="9.652" y2="2.54" width="0.127" layer="51"/>
<wire x1="9.652" y1="2.54" x2="8.128" y2="2.54" width="0.127" layer="51"/>
<wire x1="8.89" y1="3.302" x2="9.652" y2="2.54" width="0.127" layer="51"/>
<wire x1="9.652" y1="2.54" x2="10.414" y2="3.302" width="0.127" layer="51"/>
<wire x1="11.43" y1="3.302" x2="12.192" y2="2.54" width="0.127" layer="51"/>
<wire x1="12.192" y1="2.54" x2="12.954" y2="3.302" width="0.127" layer="51"/>
<smd name="P$1" x="-2.54" y="0" dx="1.778" dy="3.81" layer="1"/>
<smd name="P$2" x="-2.54" y="0" dx="1.778" dy="3.81" layer="16"/>
<smd name="P$3" x="0" y="0" dx="1.778" dy="3.81" layer="1"/>
<smd name="P$4" x="0" y="0" dx="1.778" dy="3.81" layer="16"/>
<smd name="P$5" x="2.54" y="0" dx="1.778" dy="3.81" layer="1"/>
<smd name="P$6" x="2.54" y="0" dx="1.778" dy="3.81" layer="16"/>
<text x="-13.716" y="3.81" size="1.27" layer="51" font="vector">Board</text>
<text x="8.128" y="3.81" size="1.27" layer="51" font="vector">Board</text>
<rectangle x1="-2.794" y1="-0.762" x2="-2.286" y2="2.54" layer="51"/>
<rectangle x1="-0.254" y1="-0.762" x2="0.254" y2="2.54" layer="51"/>
<rectangle x1="2.286" y1="-0.762" x2="2.794" y2="2.54" layer="51"/>
<polygon width="0.127" layer="51">
<vertex x="-0.254" y="4.826"/>
<vertex x="-0.254" y="10.668"/>
<vertex x="0" y="10.922"/>
<vertex x="0.254" y="10.668"/>
<vertex x="0.254" y="4.826"/>
</polygon>
</package>
<package name="HDR_1X2">
<description>&lt;b&gt;2 pin header&lt;/b&gt;&lt;p&gt;

1x2 pin SIL header (100mil / 2.54mm pin-spacing), e.g. for jumper.

&lt;p&gt;Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<wire x1="-1.016" y1="1.27" x2="1.016" y2="1.27" width="0.2032" layer="21"/>
<wire x1="1.016" y1="1.27" x2="1.27" y2="1.016" width="0.2032" layer="21"/>
<wire x1="1.27" y1="-1.016" x2="1.016" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="1.27" y1="1.016" x2="1.524" y2="1.27" width="0.2032" layer="21"/>
<wire x1="1.524" y1="1.27" x2="3.556" y2="1.27" width="0.2032" layer="21"/>
<wire x1="3.556" y1="-1.27" x2="1.524" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="1.524" y1="-1.27" x2="1.27" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="-1.27" y1="1.016" x2="-1.27" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="-1.016" y1="1.27" x2="-1.27" y2="1.016" width="0.2032" layer="21"/>
<wire x1="-1.27" y1="-1.016" x2="-1.016" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="1.016" y1="-1.27" x2="-1.016" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="3.81" y1="1.016" x2="3.81" y2="-1.016" width="0.2032" layer="21"/>
<wire x1="3.556" y1="1.27" x2="3.81" y2="1.016" width="0.2032" layer="21"/>
<wire x1="3.81" y1="-1.016" x2="3.556" y2="-1.27" width="0.2032" layer="21"/>
<pad name="1" x="0" y="0" drill="1.016" diameter="1.778" shape="square"/>
<pad name="2" x="2.54" y="0" drill="1.016" diameter="1.778"/>
<text x="-1.27" y="1.651" size="1.27" layer="25" font="vector" ratio="16">&gt;NAME</text>
<text x="-1.27" y="-2.921" size="1.27" layer="27" font="vector" ratio="16">&gt;VALUE</text>
<text x="0.762" y="-0.254" size="0.6096" layer="51" font="vector" ratio="10">&gt;NAME</text>
<text x="-0.254" y="-0.254" size="0.6096" layer="51" font="vector" ratio="10">1</text>
</package>
<package name="MOLEX_KK_HDR2_FL">
<description>&lt;b&gt;Molex KK 2.54mm straight male connector, 2 pin, friction lock&lt;/b&gt;&lt;p&gt;
Molex part no: 22-27-2021 (tin), 22-29-2021 (gold)&lt;p&gt;
Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<wire x1="0" y1="2.8575" x2="0" y2="1.8415" width="0.127" layer="21"/>
<wire x1="0" y1="1.8415" x2="2.54" y2="1.8415" width="0.127" layer="21"/>
<wire x1="-1.27" y1="2.8575" x2="0" y2="2.8575" width="0.127" layer="21"/>
<wire x1="0" y1="2.8575" x2="2.54" y2="2.8575" width="0.127" layer="21"/>
<wire x1="2.54" y1="2.8575" x2="3.81" y2="2.8575" width="0.127" layer="21"/>
<wire x1="-1.27" y1="-3.2385" x2="3.81" y2="-3.2385" width="0.127" layer="21"/>
<wire x1="3.81" y1="-3.2385" x2="3.81" y2="2.8575" width="0.127" layer="21"/>
<wire x1="-1.27" y1="2.8575" x2="-1.27" y2="-3.2385" width="0.127" layer="21"/>
<wire x1="2.54" y1="1.8415" x2="2.54" y2="2.8575" width="0.127" layer="21"/>
<pad name="1" x="0" y="0" drill="1.016" diameter="1.778" shape="square"/>
<pad name="2" x="2.54" y="0" drill="1.016" diameter="1.778"/>
</package>
</packages>
<symbols>
<symbol name="MA1X16">
<wire x1="-3.81" y1="-2.54" x2="-8.89" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="5.08" x2="-5.08" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="2.54" x2="-5.08" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="0" x2="-5.08" y2="0" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="10.16" x2="-5.08" y2="10.16" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="7.62" x2="-5.08" y2="7.62" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="17.78" x2="-5.08" y2="17.78" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="15.24" x2="-5.08" y2="15.24" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="12.7" x2="-5.08" y2="12.7" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="22.86" x2="-5.08" y2="22.86" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="20.32" x2="-5.08" y2="20.32" width="0.6096" layer="94"/>
<wire x1="-8.89" y1="40.64" x2="-8.89" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="-2.54" x2="-3.81" y2="40.64" width="0.4064" layer="94"/>
<wire x1="-8.89" y1="40.64" x2="-3.81" y2="40.64" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="30.48" x2="-5.08" y2="30.48" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="27.94" x2="-5.08" y2="27.94" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="25.4" x2="-5.08" y2="25.4" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="35.56" x2="-5.08" y2="35.56" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="33.02" x2="-5.08" y2="33.02" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="38.1" x2="-5.08" y2="38.1" width="0.6096" layer="94"/>
<text x="-8.89" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<text x="-8.89" y="41.402" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="0" y="0" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="2" x="0" y="2.54" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="3" x="0" y="5.08" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="4" x="0" y="7.62" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="5" x="0" y="10.16" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="6" x="0" y="12.7" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="7" x="0" y="15.24" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="8" x="0" y="17.78" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="9" x="0" y="20.32" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="10" x="0" y="22.86" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="11" x="0" y="25.4" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="12" x="0" y="27.94" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="13" x="0" y="30.48" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="14" x="0" y="33.02" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="15" x="0" y="35.56" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="16" x="0" y="38.1" visible="pad" length="middle" direction="pas" rot="R180"/>
</symbol>
<symbol name="MA2X3">
<wire x1="-3.81" y1="-2.54" x2="-11.43" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="5.08" x2="-5.08" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="2.54" x2="-5.08" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="0" x2="-5.08" y2="0" width="0.6096" layer="94"/>
<wire x1="-10.16" y1="5.08" x2="-8.89" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-10.16" y1="2.54" x2="-8.89" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-10.16" y1="0" x2="-8.89" y2="0" width="0.6096" layer="94"/>
<wire x1="-11.43" y1="7.62" x2="-11.43" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="-2.54" x2="-3.81" y2="7.62" width="0.4064" layer="94"/>
<wire x1="-11.43" y1="7.62" x2="-3.81" y2="7.62" width="0.4064" layer="94"/>
<text x="-11.43" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<text x="-11.43" y="8.382" size="1.778" layer="95">&gt;NAME</text>
<text x="-3.048" y="0.254" size="1.27" layer="95">1</text>
<text x="-13.208" y="0.254" size="1.27" layer="95">2</text>
<text x="-3.048" y="2.794" size="1.27" layer="95">3</text>
<text x="-13.208" y="2.794" size="1.27" layer="95">4</text>
<text x="-3.048" y="5.334" size="1.27" layer="95">5</text>
<text x="-13.208" y="5.334" size="1.27" layer="95">6</text>
<pin name="1" x="0" y="0" visible="off" length="middle" direction="pas" rot="R180"/>
<pin name="3" x="0" y="2.54" visible="off" length="middle" direction="pas" rot="R180"/>
<pin name="5" x="0" y="5.08" visible="off" length="middle" direction="pas" rot="R180"/>
<pin name="2" x="-15.24" y="0" visible="off" length="middle" direction="pas"/>
<pin name="4" x="-15.24" y="2.54" visible="off" length="middle" direction="pas"/>
<pin name="6" x="-15.24" y="5.08" visible="off" length="middle" direction="pas"/>
</symbol>
<symbol name="MA1X2">
<wire x1="-3.81" y1="-2.54" x2="-8.89" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="2.54" x2="-5.08" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-6.35" y1="0" x2="-5.08" y2="0" width="0.6096" layer="94"/>
<wire x1="-8.89" y1="5.08" x2="-8.89" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="-2.54" x2="-3.81" y2="5.08" width="0.4064" layer="94"/>
<wire x1="-8.89" y1="5.08" x2="-3.81" y2="5.08" width="0.4064" layer="94"/>
<text x="-8.89" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<text x="-8.89" y="5.842" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="0" y="0" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="2" x="0" y="2.54" visible="pad" length="middle" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="HEADER2X8L" prefix="CON">
<description>&lt;b&gt;16 pin (2x8) header&lt;/b&gt;&lt;p&gt;

One-row schematics symbol</description>
<gates>
<gate name="G$1" symbol="MA1X16" x="0" y="0"/>
</gates>
<devices>
<device name="" package="HDR_2X8">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="11" pad="11"/>
<connect gate="G$1" pin="12" pad="12"/>
<connect gate="G$1" pin="13" pad="13"/>
<connect gate="G$1" pin="14" pad="14"/>
<connect gate="G$1" pin="15" pad="15"/>
<connect gate="G$1" pin="16" pad="16"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="S" package="HDR_S_2X8">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="11" pad="11"/>
<connect gate="G$1" pin="12" pad="12"/>
<connect gate="G$1" pin="13" pad="13"/>
<connect gate="G$1" pin="14" pad="14"/>
<connect gate="G$1" pin="15" pad="15"/>
<connect gate="G$1" pin="16" pad="16"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="HEADER2X3" prefix="CON">
<description>&lt;b&gt;6 pin (2x3) header&lt;/b&gt;&lt;p&gt;

Two-row schematics symbol</description>
<gates>
<gate name="G$1" symbol="MA2X3" x="0" y="0"/>
</gates>
<devices>
<device name="" package="HDR_2X3">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="S" package="HDR_S_2X3">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SE" package="HDR_SE_2X3">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
<connect gate="G$1" pin="3" pad="P$3"/>
<connect gate="G$1" pin="4" pad="P$4"/>
<connect gate="G$1" pin="5" pad="P$5"/>
<connect gate="G$1" pin="6" pad="P$6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="HEADER1X2" prefix="CON">
<description>&lt;b&gt;2 pin SIL header&lt;/b&gt;&lt;p&gt;

100mil / 2.54mm pin-spacing.

&lt;p&gt;Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<gates>
<gate name="G$1" symbol="MA1X2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="HDR_1X2">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="KK" package="MOLEX_KK_HDR2_FL">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="cap">
<packages>
<package name="C0402">
<description>&lt;b&gt;Chip capacitor 0402 reflow&lt;/b&gt;&lt;p&gt;
Size 0402 (IPC 1005) chip capacitor. Reflow soldering footprint.</description>
<wire x1="-1.473" y1="0.483" x2="1.473" y2="0.483" width="0.0508" layer="39"/>
<wire x1="1.473" y1="0.483" x2="1.473" y2="-0.483" width="0.0508" layer="39"/>
<wire x1="1.473" y1="-0.483" x2="-1.473" y2="-0.483" width="0.0508" layer="39"/>
<wire x1="-1.473" y1="-0.483" x2="-1.473" y2="0.483" width="0.0508" layer="39"/>
<wire x1="-0.55" y1="0.3" x2="0.55" y2="0.3" width="0.0508" layer="51"/>
<wire x1="0.55" y1="0.3" x2="0.55" y2="-0.3" width="0.0508" layer="51"/>
<wire x1="0.55" y1="-0.3" x2="-0.55" y2="-0.3" width="0.0508" layer="51"/>
<wire x1="-0.55" y1="-0.3" x2="-0.55" y2="0.3" width="0.0508" layer="51"/>
<smd name="1" x="-0.65" y="0" dx="0.9" dy="0.7" layer="1"/>
<smd name="2" x="0.65" y="0" dx="0.9" dy="0.7" layer="1"/>
<text x="-0.635" y="0.762" size="1.27" layer="25">&gt;NAME</text>
<text x="-0.635" y="-1.905" size="1.27" layer="27">&gt;VALUE</text>
<text x="-0.45" y="-0.15" size="0.254" layer="51">&gt;NAME</text>
<rectangle x1="-0.1999" y1="-0.3" x2="0.1999" y2="0.3" layer="35"/>
</package>
<package name="C0603">
<description>&lt;b&gt;Chip capacitor 0603 reflow&lt;/b&gt;&lt;p&gt;
Size 0603 (IPC 1608) chip capacitor. Reflow soldering footprint.</description>
<wire x1="-1.473" y1="0.983" x2="1.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.473" y1="0.983" x2="1.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="1.473" y1="-0.983" x2="-1.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.473" y1="-0.983" x2="-1.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="-0.8" y1="0.45" x2="0.8" y2="0.45" width="0.0508" layer="51"/>
<wire x1="0.8" y1="0.45" x2="0.8" y2="-0.45" width="0.0508" layer="51"/>
<wire x1="0.8" y1="-0.45" x2="-0.8" y2="-0.45" width="0.0508" layer="51"/>
<wire x1="-0.8" y1="-0.45" x2="-0.8" y2="0.45" width="0.0508" layer="51"/>
<smd name="1" x="-0.85" y="0" dx="1.1" dy="1" layer="1"/>
<smd name="2" x="0.85" y="0" dx="1.1" dy="1" layer="1"/>
<text x="-0.889" y="0.762" size="1.27" layer="25">&gt;NAME</text>
<text x="-0.889" y="-2.032" size="1.27" layer="27">&gt;VALUE</text>
<text x="-0.7" y="-0.15" size="0.3048" layer="51">&gt;NAME</text>
<rectangle x1="-0.1999" y1="-0.3" x2="0.1999" y2="0.3" layer="35"/>
</package>
<package name="C0805">
<description>&lt;b&gt;Chip capacitor 0805 reflow&lt;/b&gt;&lt;p&gt;
Size 0805 (IPC 2012) chip capacitor. Reflow soldering footprint.</description>
<wire x1="-1.973" y1="0.983" x2="1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="-0.983" x2="-1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.973" y1="-0.983" x2="-1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="0.983" x2="1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.05" y1="0.7" x2="1.05" y2="0.7" width="0.0508" layer="51"/>
<wire x1="1.05" y1="0.7" x2="1.05" y2="-0.7" width="0.0508" layer="51"/>
<wire x1="1.05" y1="-0.7" x2="-1.05" y2="-0.7" width="0.0508" layer="51"/>
<wire x1="-1.05" y1="-0.7" x2="-1.05" y2="0.7" width="0.0508" layer="51"/>
<smd name="1" x="-0.95" y="0" dx="1.3" dy="1.5" layer="1"/>
<smd name="2" x="0.95" y="0" dx="1.3" dy="1.5" layer="1"/>
<text x="-1.27" y="1.27" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.54" size="1.27" layer="27">&gt;VALUE</text>
<text x="-0.9" y="-0.2" size="0.4064" layer="51">&gt;NAME</text>
<rectangle x1="-0.1001" y1="-0.4001" x2="0.1001" y2="0.4001" layer="35"/>
</package>
<package name="C1210">
<description>&lt;b&gt;Chip capacitor 1210 reflow&lt;/b&gt;&lt;p&gt;
Size 1210 (IPC 3225) chip capacitor. Reflow soldering footprint.</description>
<wire x1="-2.473" y1="1.483" x2="2.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="1.483" x2="2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-1.483" x2="-2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-1.483" x2="-2.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="-1.6" y1="1.25" x2="1.6" y2="1.25" width="0.0508" layer="51"/>
<wire x1="1.6" y1="1.25" x2="1.6" y2="-1.25" width="0.0508" layer="51"/>
<wire x1="1.6" y1="-1.25" x2="-1.6" y2="-1.25" width="0.0508" layer="51"/>
<wire x1="-1.6" y1="-1.25" x2="-1.6" y2="1.25" width="0.0508" layer="51"/>
<smd name="1" x="-1.4" y="0" dx="1.6" dy="2.7" layer="1"/>
<smd name="2" x="1.4" y="0" dx="1.6" dy="2.7" layer="1"/>
<text x="-1.27" y="1.651" size="1.27" layer="25" font="vector">&gt;NAME</text>
<text x="-1.27" y="-2.921" size="1.27" layer="27" font="vector">&gt;VALUE</text>
<text x="-1.35" y="-0.3" size="0.6096" layer="51" font="vector">&gt;NAME</text>
<rectangle x1="-0.3" y1="-0.8999" x2="0.3" y2="0.8999" layer="35"/>
</package>
<package name="C1206">
<description>&lt;b&gt;Chip capacitor 1206 reflow&lt;/b&gt;&lt;p&gt;
Size 1206 (IPC 3216) chip capacitor. Reflow soldering footprint.</description>
<wire x1="-2.473" y1="0.983" x2="2.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="0.983" x2="2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-0.983" x2="-2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-0.983" x2="-2.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="-1.65" y1="0.85" x2="-1.65" y2="-0.85" width="0.0508" layer="51"/>
<wire x1="-1.65" y1="-0.85" x2="1.65" y2="-0.85" width="0.0508" layer="51"/>
<wire x1="1.65" y1="-0.85" x2="1.65" y2="0.85" width="0.0508" layer="51"/>
<wire x1="1.65" y1="0.85" x2="-1.65" y2="0.85" width="0.0508" layer="51"/>
<smd name="2" x="1.4" y="0" dx="1.6" dy="1.8" layer="1"/>
<smd name="1" x="-1.4" y="0" dx="1.6" dy="1.8" layer="1"/>
<text x="-1.397" y="1.143" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.397" y="-2.413" size="1.27" layer="27">&gt;VALUE</text>
<text x="-1.35" y="-0.3" size="0.6096" layer="51" font="vector">&gt;NAME</text>
<rectangle x1="-0.3" y1="-0.7" x2="0.3" y2="0.7" layer="35"/>
</package>
</packages>
<symbols>
<symbol name="C-EU">
<wire x1="0" y1="0" x2="0" y2="-0.508" width="0.1524" layer="94"/>
<wire x1="0" y1="-2.54" x2="0" y2="-2.032" width="0.1524" layer="94"/>
<text x="1.524" y="0.381" size="1.778" layer="95">&gt;NAME</text>
<text x="1.524" y="-4.699" size="1.778" layer="96">&gt;VALUE</text>
<rectangle x1="-2.032" y1="-2.032" x2="2.032" y2="-1.524" layer="94"/>
<rectangle x1="-2.032" y1="-1.016" x2="2.032" y2="-0.508" layer="94"/>
<pin name="1" x="0" y="2.54" visible="off" length="short" direction="pas" swaplevel="1" rot="R270"/>
<pin name="2" x="0" y="-5.08" visible="off" length="short" direction="pas" swaplevel="1" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="C-EU_" prefix="C" uservalue="yes">
<description>&lt;h1&gt;Capacitor non-polarized (EU symbol)&lt;/h1&gt;</description>
<gates>
<gate name="G$1" symbol="C-EU" x="0" y="0" addlevel="always"/>
</gates>
<devices>
<device name="0402R" package="C0402">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="CAPACITOR_CERAMIC"/>
<attribute name="FOOTPRINT" value="0402"/>
<attribute name="NOMOUNT" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="0603R" package="C0603">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="CAPACITOR_CERAMIC"/>
<attribute name="FOOTPRINT" value="0603"/>
<attribute name="NOMOUNT" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="0805R" package="C0805">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="CAPACITOR_CERAMIC"/>
<attribute name="FOOTPRINT" value="0805"/>
<attribute name="NOMOUNT" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="1210R" package="C1210">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="CAPACITOR_CERAMIC"/>
<attribute name="FOOTPRINT" value="1210"/>
<attribute name="NOMOUNT" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="1206R" package="C1206">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="CAPACITOR_CERAMIC"/>
<attribute name="FOOTPRINT" value="1206"/>
<attribute name="NOMOUNT" value="" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply">
<packages>
</packages>
<symbols>
<symbol name="VCC">
<wire x1="-2.032" y1="2.54" x2="2.032" y2="2.54" width="0.254" layer="94"/>
<text x="-2.032" y="3.048" size="1.4224" layer="96">&gt;VALUE</text>
<pin name="VCC" x="0" y="0" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="CGND">
<wire x1="-2.54" y1="-2.54" x2="-1.27" y2="-2.54" width="0.254" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="1.27" y2="-2.54" width="0.254" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="2.54" y2="-2.54" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-2.54" x2="-3.048" y2="-4.318" width="0.127" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="2.032" y2="-4.318" width="0.127" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="-1.778" y2="-4.318" width="0.127" layer="94"/>
<wire x1="0" y1="-2.54" x2="-0.508" y2="-4.318" width="0.127" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="0.762" y2="-4.318" width="0.127" layer="94"/>
<text x="-2.794" y="-5.334" size="0.6096" layer="96">Chassis GND</text>
<pin name="P$1" x="0" y="0" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="VCC" prefix="VCC">
<description>Positive supply voltage</description>
<gates>
<gate name="VCC" symbol="VCC" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="CGND" prefix="CGND">
<description>Chassis ground</description>
<gates>
<gate name="G$1" symbol="CGND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="crystal">
<packages>
<package name="HC49SMX">
<description>&lt;h1&gt;HC49SMX SMD crystal&lt;/h1&gt;</description>
<wire x1="3.429" y1="-2.032" x2="5.109" y2="-1.1429" width="0.0508" layer="51" curve="55.771157"/>
<wire x1="5.715" y1="-1.143" x2="5.715" y2="-2.159" width="0.2032" layer="21"/>
<wire x1="-5.1091" y1="-1.143" x2="-3.429" y2="-2.032" width="0.0508" layer="51" curve="55.772485"/>
<wire x1="-5.715" y1="-1.143" x2="-5.715" y2="-2.159" width="0.2032" layer="21"/>
<wire x1="-3.429" y1="1.27" x2="3.429" y2="1.27" width="0.0508" layer="51"/>
<wire x1="-3.429" y1="2.032" x2="3.429" y2="2.032" width="0.0508" layer="51"/>
<wire x1="3.429" y1="-1.27" x2="-3.429" y2="-1.27" width="0.0508" layer="51"/>
<wire x1="-5.461" y1="2.413" x2="5.461" y2="2.413" width="0.2032" layer="21"/>
<wire x1="-5.715" y1="0.381" x2="-6.477" y2="0.381" width="0.1524" layer="51"/>
<wire x1="-5.715" y1="-0.381" x2="-6.477" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="-6.477" y1="0.381" x2="-6.477" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="-5.715" y1="2.159" x2="-5.461" y2="2.413" width="0.2032" layer="21" curve="-90"/>
<wire x1="-5.715" y1="2.159" x2="-5.715" y2="1.143" width="0.2032" layer="21"/>
<wire x1="-3.9826" y1="1.143" x2="-3.429" y2="1.27" width="0.0508" layer="51" curve="-25.842828"/>
<wire x1="-3.9826" y1="-1.143" x2="-3.429" y2="-1.27" width="0.0508" layer="51" curve="25.842828"/>
<wire x1="-5.1091" y1="1.143" x2="-3.429" y2="2.0321" width="0.0508" layer="51" curve="-55.770993"/>
<wire x1="-3.9826" y1="1.143" x2="-3.9826" y2="-1.143" width="0.0508" layer="51" curve="128.314524"/>
<wire x1="-5.1091" y1="1.143" x2="-5.1091" y2="-1.143" width="0.0508" layer="51" curve="68.456213"/>
<wire x1="3.429" y1="2.032" x2="5.1091" y2="1.143" width="0.0508" layer="51" curve="-55.772485"/>
<wire x1="3.9826" y1="1.143" x2="3.9826" y2="-1.143" width="0.0508" layer="51" curve="-128.314524"/>
<wire x1="3.429" y1="1.27" x2="3.9826" y2="1.143" width="0.0508" layer="51" curve="-25.842828"/>
<wire x1="3.429" y1="-1.27" x2="3.9826" y2="-1.143" width="0.0508" layer="51" curve="25.842828"/>
<wire x1="6.477" y1="0.381" x2="6.477" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="5.1091" y1="1.143" x2="5.1091" y2="-1.143" width="0.0508" layer="51" curve="-68.456213"/>
<wire x1="5.715" y1="0.381" x2="5.715" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="5.715" y1="2.159" x2="5.715" y2="1.143" width="0.2032" layer="21"/>
<wire x1="5.461" y1="2.413" x2="5.715" y2="2.159" width="0.2032" layer="21" curve="-90"/>
<wire x1="5.715" y1="0.381" x2="6.477" y2="0.381" width="0.1524" layer="51"/>
<wire x1="5.715" y1="-0.381" x2="6.477" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="3.429" y1="-2.032" x2="-3.429" y2="-2.032" width="0.0508" layer="51"/>
<wire x1="-5.461" y1="-2.413" x2="5.461" y2="-2.413" width="0.2032" layer="21"/>
<wire x1="-5.715" y1="-2.159" x2="-5.461" y2="-2.413" width="0.2032" layer="21" curve="90"/>
<wire x1="5.461" y1="-2.413" x2="5.715" y2="-2.159" width="0.2032" layer="21" curve="90"/>
<wire x1="5.715" y1="-0.381" x2="5.715" y2="-2.159" width="0.1524" layer="51"/>
<wire x1="-5.715" y1="2.159" x2="-5.715" y2="-2.159" width="0.1524" layer="51"/>
<wire x1="-5.461" y1="2.413" x2="5.461" y2="2.413" width="0.1524" layer="51"/>
<wire x1="-5.715" y1="2.159" x2="-5.461" y2="2.413" width="0.1524" layer="51" curve="-90"/>
<wire x1="5.715" y1="2.159" x2="5.715" y2="0.381" width="0.1524" layer="51"/>
<wire x1="5.461" y1="2.413" x2="5.715" y2="2.159" width="0.1524" layer="51" curve="-90"/>
<wire x1="-5.461" y1="-2.413" x2="5.461" y2="-2.413" width="0.1524" layer="51"/>
<wire x1="-5.715" y1="-2.159" x2="-5.461" y2="-2.413" width="0.1524" layer="51" curve="90"/>
<wire x1="5.461" y1="-2.413" x2="5.715" y2="-2.159" width="0.1524" layer="51" curve="90"/>
<smd name="1" x="-4.826" y="0" dx="5.08" dy="1.778" layer="1"/>
<smd name="2" x="4.826" y="0" dx="5.08" dy="1.778" layer="1"/>
<text x="-5.715" y="2.667" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-5.715" y="-4.064" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="-3.175" y="-0.635" size="1.27" layer="51">&gt;NAME</text>
<rectangle x1="-6.604" y1="-2.54" x2="6.604" y2="2.794" layer="43"/>
</package>
<package name="HC49">
<description>&lt;B&gt;CRYSTAL&lt;/B&gt;</description>
<wire x1="-3.429" y1="-2.286" x2="3.429" y2="-2.286" width="0.254" layer="21"/>
<wire x1="3.429" y1="2.286" x2="-3.429" y2="2.286" width="0.254" layer="21"/>
<wire x1="-3.429" y1="-1.778" x2="3.429" y2="-1.778" width="0.0508" layer="51"/>
<wire x1="3.429" y1="1.778" x2="-3.429" y2="1.778" width="0.0508" layer="51"/>
<wire x1="3.429" y1="1.778" x2="3.429" y2="-1.778" width="0.0508" layer="51" curve="-180"/>
<wire x1="3.429" y1="2.286" x2="3.429" y2="-2.286" width="0.254" layer="21" curve="-180"/>
<wire x1="-3.429" y1="2.286" x2="-3.429" y2="-2.286" width="0.254" layer="21" curve="180"/>
<wire x1="-3.429" y1="1.778" x2="-3.429" y2="-1.778" width="0.0508" layer="51" curve="180"/>
<pad name="1" x="-2.54" y="0" drill="0.7112" shape="long" rot="R90"/>
<pad name="2" x="2.54" y="0" drill="0.7112" shape="long" rot="R90"/>
<text x="-5.08" y="-3.937" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="-5.08" y="2.667" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.778" y="-0.381" size="0.8128" layer="51" font="vector">&gt;NAME</text>
</package>
</packages>
<symbols>
<symbol name="XT">
<wire x1="0" y1="-1.016" x2="0" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="0" y1="2.54" x2="0" y2="1.016" width="0.1524" layer="94"/>
<wire x1="1.524" y1="0.381" x2="-1.524" y2="0.381" width="0.254" layer="94"/>
<wire x1="-1.524" y1="0.381" x2="-1.524" y2="-0.381" width="0.254" layer="94"/>
<wire x1="-1.524" y1="-0.381" x2="1.524" y2="-0.381" width="0.254" layer="94"/>
<wire x1="1.524" y1="-0.381" x2="1.524" y2="0.381" width="0.254" layer="94"/>
<wire x1="1.778" y1="-1.016" x2="-1.778" y2="-1.016" width="0.254" layer="94"/>
<wire x1="1.778" y1="1.016" x2="-1.778" y2="1.016" width="0.254" layer="94"/>
<text x="2.54" y="1.016" size="1.778" layer="95">&gt;NAME</text>
<text x="2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<text x="-2.159" y="-1.143" size="0.8636" layer="93">1</text>
<text x="1.524" y="-1.143" size="0.8636" layer="93">2</text>
<pin name="2" x="0" y="-2.54" visible="off" length="point" direction="pas" swaplevel="1" rot="R90"/>
<pin name="1" x="0" y="2.54" visible="off" length="point" direction="pas" swaplevel="1" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="XTAL_" prefix="XT" uservalue="yes">
<description>&lt;h1&gt;Crystal&lt;/h1&gt;</description>
<gates>
<gate name="G$1" symbol="XT" x="0" y="0"/>
</gates>
<devices>
<device name="HC49SMX" package="HC49SMX">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="XTAL"/>
<attribute name="FOOTPRINT" value="HC49SMX"/>
<attribute name="NOMOUNT" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="HC49" package="HC49">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="fet">
<packages>
<package name="SOT23">
<description>&lt;b&gt;SOT23 (TO-236)&lt;/b&gt;</description>
<wire x1="1.4224" y1="0.6604" x2="1.4224" y2="-0.6604" width="0.127" layer="51"/>
<wire x1="1.4224" y1="-0.6604" x2="-1.4224" y2="-0.6604" width="0.127" layer="51"/>
<wire x1="-1.4224" y1="-0.6604" x2="-1.4224" y2="0.6604" width="0.127" layer="51"/>
<wire x1="-1.4224" y1="0.6604" x2="1.4224" y2="0.6604" width="0.127" layer="51"/>
<wire x1="-0.762" y1="0.6604" x2="-1.4224" y2="0.6604" width="0.2032" layer="21"/>
<wire x1="-1.4224" y1="0.6604" x2="-1.4224" y2="-0.127" width="0.2032" layer="21"/>
<wire x1="0.762" y1="0.6604" x2="1.4224" y2="0.6604" width="0.2032" layer="21"/>
<wire x1="1.4224" y1="0.6604" x2="1.4224" y2="-0.127" width="0.2032" layer="21"/>
<smd name="3" x="0" y="1.1" dx="1" dy="1.4" layer="1"/>
<smd name="2" x="0.95" y="-1.1" dx="1" dy="1.4" layer="1"/>
<smd name="1" x="-0.95" y="-1.1" dx="1" dy="1.4" layer="1"/>
<text x="-1.905" y="2.159" size="1.27" layer="25" ratio="16">&gt;NAME</text>
<text x="-1.905" y="-3.429" size="1.27" layer="27" ratio="16">&gt;VALUE</text>
<text x="-1.27" y="-0.381" size="0.6096" layer="51">&gt;NAME</text>
<rectangle x1="-0.2286" y1="0.7112" x2="0.2286" y2="1.2954" layer="51"/>
<rectangle x1="0.7112" y1="-1.2954" x2="1.1684" y2="-0.7112" layer="51"/>
<rectangle x1="-1.1684" y1="-1.2954" x2="-0.7112" y2="-0.7112" layer="51"/>
</package>
<package name="TO92L">
<description>&lt;b&gt;TO-92&lt;/b&gt; Pads In Line</description>
<wire x1="-2.095" y1="-1.651" x2="2.095" y2="-1.651" width="0.127" layer="21"/>
<wire x1="-2.413" y1="1.1359" x2="2.413" y2="1.1359" width="0.127" layer="21" curve="-129.583345" cap="flat"/>
<wire x1="1.136" y1="-0.127" x2="-1.136" y2="-0.127" width="0.127" layer="51"/>
<wire x1="-2.413" y1="1.1359" x2="-2.413" y2="-1.1359" width="0.127" layer="51" curve="50.416655" cap="flat"/>
<wire x1="-1.404" y1="-0.127" x2="-2.664" y2="-0.127" width="0.127" layer="51"/>
<wire x1="-2.4135" y1="-1.1359" x2="-2.095" y2="-1.651" width="0.127" layer="21" curve="13.038528" cap="flat"/>
<wire x1="-1.136" y1="-0.127" x2="-1.404" y2="-0.127" width="0.127" layer="21"/>
<wire x1="2.413" y1="-1.1359" x2="2.413" y2="1.1359" width="0.127" layer="51" curve="50.416655" cap="flat"/>
<wire x1="2.664" y1="-0.127" x2="1.404" y2="-0.127" width="0.127" layer="51"/>
<wire x1="1.404" y1="-0.127" x2="1.136" y2="-0.127" width="0.127" layer="21"/>
<wire x1="2.095" y1="-1.651" x2="2.4247" y2="-1.1118" width="0.127" layer="21" curve="13.609443" cap="flat"/>
<pad name="2" x="0" y="0" drill="0.8128" shape="long" rot="R90"/>
<pad name="1" x="-2.54" y="0" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="2.54" y="0" drill="0.8128" shape="long" rot="R90"/>
<text x="-2.54" y="3.048" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-3.302" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="FET_N">
<wire x1="-1.27" y1="0" x2="-0.254" y2="0.381" width="0.1524" layer="94"/>
<wire x1="-0.254" y1="0.381" x2="-0.254" y2="-0.381" width="0.1524" layer="94"/>
<wire x1="-0.254" y1="-0.381" x2="-1.27" y2="0" width="0.1524" layer="94"/>
<wire x1="-1.27" y1="0" x2="-1.016" y2="0" width="0.1524" layer="94"/>
<wire x1="-1.016" y1="0" x2="-0.889" y2="0" width="0.1524" layer="94"/>
<wire x1="-0.889" y1="0" x2="0" y2="0" width="0.1524" layer="94"/>
<wire x1="0" y1="0" x2="0" y2="-2.032" width="0.1524" layer="94"/>
<wire x1="0" y1="-2.032" x2="0" y2="-2.794" width="0.1524" layer="94"/>
<wire x1="-1.524" y1="0" x2="-1.27" y2="0" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="2.54" x2="-2.54" y2="-2.54" width="0.254" layer="94"/>
<wire x1="-1.524" y1="2.032" x2="0" y2="2.032" width="0.1524" layer="94"/>
<wire x1="0" y1="2.032" x2="0" y2="2.54" width="0.1524" layer="94"/>
<wire x1="-1.524" y1="-2.032" x2="0" y2="-2.032" width="0.1524" layer="94"/>
<wire x1="-1.016" y1="0" x2="-0.381" y2="-0.254" width="0.254" layer="94"/>
<wire x1="-0.381" y1="-0.254" x2="-0.381" y2="0.254" width="0.254" layer="94"/>
<wire x1="-0.381" y1="0.254" x2="-0.889" y2="0" width="0.254" layer="94"/>
<circle x="0" y="-2.032" radius="0.3592" width="0" layer="94"/>
<text x="0.762" y="0.254" size="1.778" layer="95">&gt;NAME</text>
<text x="0.762" y="-2.032" size="1.778" layer="96">&gt;VALUE</text>
<rectangle x1="-2.032" y1="1.27" x2="-1.524" y2="2.54" layer="94"/>
<rectangle x1="-2.032" y1="-2.54" x2="-1.524" y2="-1.27" layer="94"/>
<rectangle x1="-2.032" y1="-0.762" x2="-1.524" y2="0.762" layer="94"/>
<pin name="G" x="-5.08" y="-2.54" visible="off" length="short" direction="pas"/>
<pin name="D" x="0" y="5.08" visible="off" length="short" direction="pas" rot="R270"/>
<pin name="S" x="0" y="-5.08" visible="off" length="short" direction="pas" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="2N700?" prefix="T">
<description>&lt;b&gt;N-channel Enhancement Mode Field Effect Transistor&lt;/b&gt;&lt;p&gt;

2N7002 by Fairchild Semiconductor&lt;p&gt;

simple schematics symbol&lt;p&gt;

&lt;p&gt;Author: &lt;author&gt;Mikkel Holm Olsen &amp;lt;eagle@symlink.dk&amp;gt;&lt;/author&gt;</description>
<gates>
<gate name="G$1" symbol="FET_N" x="0" y="0"/>
</gates>
<devices>
<device name="2" package="SOT23">
<connects>
<connect gate="G$1" pin="D" pad="3"/>
<connect gate="G$1" pin="G" pad="1"/>
<connect gate="G$1" pin="S" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0" package="TO92L">
<connects>
<connect gate="G$1" pin="D" pad="3"/>
<connect gate="G$1" pin="G" pad="2"/>
<connect gate="G$1" pin="S" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="leds">
<packages>
<package name="LED3MM">
<description>&lt;B&gt;LED&lt;/B&gt;&lt;p&gt;
3 mm, round</description>
<wire x1="1.25" y1="0.75" x2="1.25" y2="-0.75" width="0.127" layer="51" curve="297.710641"/>
<wire x1="1.25" y1="0.75" x2="1.25" y2="-0.75" width="0.127" layer="51"/>
<wire x1="-0.508" y1="0.762" x2="-0.127" y2="0.762" width="0.0762" layer="51"/>
<wire x1="-0.127" y1="0.762" x2="-0.127" y2="0.508" width="0.0762" layer="51"/>
<wire x1="-0.127" y1="0.508" x2="0.127" y2="0.762" width="0.0762" layer="51"/>
<wire x1="0.127" y1="0.762" x2="-0.127" y2="1.016" width="0.0762" layer="51"/>
<wire x1="-0.127" y1="1.016" x2="-0.127" y2="0.762" width="0.0762" layer="51"/>
<wire x1="0.127" y1="1.016" x2="0.127" y2="0.762" width="0.0762" layer="51"/>
<wire x1="0.127" y1="0.762" x2="0.127" y2="0.508" width="0.0762" layer="51"/>
<wire x1="0.127" y1="0.762" x2="0.508" y2="0.762" width="0.0762" layer="51"/>
<wire x1="-1.143" y1="0.889" x2="1.143" y2="0.889" width="0.2032" layer="21" curve="-104.250033"/>
<wire x1="1.143" y1="-0.889" x2="-1.143" y2="-0.889" width="0.2032" layer="21" curve="-104.250033"/>
<pad name="A" x="-1.27" y="0" drill="0.8" diameter="1.4"/>
<pad name="C" x="1.27" y="0" drill="0.8" diameter="1.4"/>
<text x="2.105" y="0.381" size="1.27" layer="25" ratio="16">&gt;NAME</text>
<text x="2.105" y="-1.651" size="1.27" layer="27" ratio="16">&gt;VALUE</text>
<text x="-1.15" y="-0.3" size="0.508" layer="51" ratio="10">&gt;NAME</text>
</package>
</packages>
<symbols>
<symbol name="LED_GREEN">
<wire x1="-1.27" y1="1.016" x2="0" y2="-1.016" width="0.254" layer="94"/>
<wire x1="0" y1="-1.016" x2="1.27" y2="1.016" width="0.254" layer="94"/>
<wire x1="1.27" y1="-1.016" x2="0" y2="-1.016" width="0.254" layer="94"/>
<wire x1="1.27" y1="1.016" x2="0" y2="1.016" width="0.254" layer="94"/>
<wire x1="0" y1="1.016" x2="-1.27" y2="1.016" width="0.254" layer="94"/>
<wire x1="0" y1="-1.016" x2="-1.27" y2="-1.016" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="0" y2="1.016" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="0" y2="-1.016" width="0.254" layer="94"/>
<wire x1="1.016" y1="-0.254" x2="1.778" y2="-0.762" width="0.127" layer="94"/>
<wire x1="1.778" y1="-0.762" x2="1.524" y2="-0.8128" width="0.127" layer="94"/>
<wire x1="1.778" y1="-0.762" x2="1.7272" y2="-0.508" width="0.127" layer="94"/>
<wire x1="1.27" y1="0.254" x2="2.032" y2="-0.254" width="0.127" layer="94"/>
<wire x1="2.032" y1="-0.254" x2="1.778" y2="-0.3048" width="0.127" layer="94"/>
<wire x1="2.032" y1="-0.254" x2="1.9812" y2="0" width="0.127" layer="94"/>
<text x="3.048" y="0.4826" size="1.778" layer="95" font="vector">&gt;NAME</text>
<text x="3.048" y="-2.3114" size="1.778" layer="96" font="vector">&gt;VALUE</text>
<text x="1.524" y="0.508" size="0.8128" layer="94" ratio="16">G</text>
<pin name="A" x="0" y="2.54" visible="off" length="point" direction="pas" rot="R270"/>
<pin name="C" x="0" y="-2.54" visible="off" length="point" direction="pas" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="LTL-1CHG" prefix="LED">
<description>&lt;h1&gt;3mm (T-1) Green LED&lt;/h1&gt;&lt;p&gt;
Mfg: Lite-On Inc&lt;p&gt;
Green, diffused, 12.6mcd@2.1V&lt;p&gt;</description>
<gates>
<gate name="G$1" symbol="LED_GREEN" x="0" y="0"/>
</gates>
<devices>
<device name="" package="LED3MM">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="DIODE"/>
<attribute name="FOOTPRINT" value="LED3MM2PIN"/>
<attribute name="NOMOUNT" value="" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U1" library="avr-mcu" deviceset="ATMEGA644P" device="" value="ATMEGA644P"/>
<part name="FRAME1" library="frame" deviceset="DINA4_L" device=""/>
<part name="J4" library="_newbylars" deviceset="DIN5_FEMALE_PCB" device="" value="MIDI IN 1"/>
<part name="J3" library="_newbylars" deviceset="DIN5_FEMALE_PCB" device="" value="MIDI IN 2"/>
<part name="J2" library="_newbylars" deviceset="DIN5_FEMALE_PCB" device="" value="MIDI OUT"/>
<part name="J5" library="con-head" deviceset="HEADER2X8L" device="S" value="LCD"/>
<part name="J9" library="_newbylars" deviceset="HEADER2X3" device="" value="ISP"/>
<part name="J1" library="_newbylars" deviceset="PN61729-S" device="" value="USB"/>
<part name="C1" library="cap" deviceset="C-EU_" device="1210R" value="10uF"/>
<part name="C2" library="cap" deviceset="C-EU_" device="0805R" value="100nF"/>
<part name="VCC1" library="supply" deviceset="VCC" device=""/>
<part name="J6" library="con-head" deviceset="HEADER2X3" device="S" value="BUTTONS"/>
<part name="R1" library="_newbylars" deviceset="R-EU" device="0805" value="68R"/>
<part name="OK1" library="_newbylars" deviceset="6N138" device="DIL08" value="6N139"/>
<part name="GND2" library="_newbylars" deviceset="GND" device=""/>
<part name="GND1" library="_newbylars" deviceset="GND" device=""/>
<part name="D1" library="_newbylars" deviceset="ZENER-DIODE" device="DO34-7" value="3.6V"/>
<part name="D2" library="_newbylars" deviceset="ZENER-DIODE" device="DO34-7" value="3.6V"/>
<part name="R2" library="_newbylars" deviceset="R-EU" device="0805" value="68R"/>
<part name="R3" library="_newbylars" deviceset="R-EU" device="0805" value="1k5"/>
<part name="R4" library="_newbylars" deviceset="R-EU" device="0805" value="NM"/>
<part name="XT1" library="crystal" deviceset="XTAL_" device="HC49" value="20 MHz"/>
<part name="C3" library="cap" deviceset="C-EU_" device="0805R" value="100nF"/>
<part name="C4" library="cap" deviceset="C-EU_" device="0805R" value="22pF"/>
<part name="C5" library="cap" deviceset="C-EU_" device="0805R" value="22pF"/>
<part name="GND6" library="_newbylars" deviceset="GND" device=""/>
<part name="C6" library="cap" deviceset="C-EU_" device="0805R" value="100nF"/>
<part name="GND7" library="_newbylars" deviceset="GND" device=""/>
<part name="GND8" library="_newbylars" deviceset="GND" device=""/>
<part name="VCC2" library="supply" deviceset="VCC" device=""/>
<part name="GND9" library="_newbylars" deviceset="GND" device=""/>
<part name="VCC3" library="supply" deviceset="VCC" device=""/>
<part name="GND10" library="_newbylars" deviceset="GND" device=""/>
<part name="VCC4" library="supply" deviceset="VCC" device=""/>
<part name="OK2" library="_newbylars" deviceset="6N138" device="DIL08" value="6N139"/>
<part name="GND11" library="_newbylars" deviceset="GND" device=""/>
<part name="R6" library="_newbylars" deviceset="R-EU" device="0805" value="1K8"/>
<part name="VCC5" library="supply" deviceset="VCC" device=""/>
<part name="R7" library="_newbylars" deviceset="R-EU" device="0805" value="1K8"/>
<part name="R8" library="_newbylars" deviceset="R-EU" device="0805" value="220R"/>
<part name="R9" library="_newbylars" deviceset="R-EU" device="0805" value="220R"/>
<part name="GND12" library="_newbylars" deviceset="GND" device=""/>
<part name="R10" library="_newbylars" deviceset="R-EU" device="0805" value="1K8"/>
<part name="R11" library="_newbylars" deviceset="R-EU" device="0805" value="1K8"/>
<part name="R12" library="_newbylars" deviceset="R-EU" device="0805" value="220R"/>
<part name="R13" library="_newbylars" deviceset="R-EU" device="0805" value="220R"/>
<part name="VCC7" library="supply" deviceset="VCC" device=""/>
<part name="VCC8" library="supply" deviceset="VCC" device=""/>
<part name="Q1" library="fet" deviceset="2N700?" device="2"/>
<part name="GND13" library="_newbylars" deviceset="GND" device=""/>
<part name="VCC6" library="supply" deviceset="VCC" device=""/>
<part name="C7" library="cap" deviceset="C-EU_" device="0805R" value="100nF"/>
<part name="GND14" library="_newbylars" deviceset="GND" device=""/>
<part name="GND15" library="_newbylars" deviceset="GND" device=""/>
<part name="J8" library="con-head" deviceset="HEADER1X2" device="KK" value="PEDAL1"/>
<part name="J7" library="con-head" deviceset="HEADER1X2" device="KK" value="PEDAL2"/>
<part name="R15" library="_newbylars" deviceset="R-EU" device="0805" value="N/M"/>
<part name="R16" library="_newbylars" deviceset="R-EU" device="0805" value="10k"/>
<part name="R17" library="_newbylars" deviceset="R-EU" device="0805" value="0R"/>
<part name="C8" library="cap" deviceset="C-EU_" device="0805R" value="47nF"/>
<part name="GND16" library="_newbylars" deviceset="GND" device=""/>
<part name="R18" library="_newbylars" deviceset="R-EU" device="0805" value="N/M"/>
<part name="R19" library="_newbylars" deviceset="R-EU" device="0805" value="10k"/>
<part name="R20" library="_newbylars" deviceset="R-EU" device="0805" value="0R"/>
<part name="C9" library="cap" deviceset="C-EU_" device="0805R" value="47nF"/>
<part name="GND17" library="_newbylars" deviceset="GND" device=""/>
<part name="VCC10" library="supply" deviceset="VCC" device=""/>
<part name="VCC9" library="supply" deviceset="VCC" device=""/>
<part name="CGND1" library="supply" deviceset="CGND" device=""/>
<part name="CGND2" library="supply" deviceset="CGND" device=""/>
<part name="CGND3" library="supply" deviceset="CGND" device=""/>
<part name="CGND4" library="supply" deviceset="CGND" device=""/>
<part name="GND3" library="_newbylars" deviceset="GND" device=""/>
<part name="CGND5" library="supply" deviceset="CGND" device=""/>
<part name="R21" library="_newbylars" deviceset="R-EU" device="0805" value="0R"/>
<part name="D9" library="leds" deviceset="LTL-1CHG" device="" value="STATUS"/>
<part name="D8" library="leds" deviceset="LTL-1CHG" device="" value="POWER"/>
<part name="R22" library="_newbylars" deviceset="R-EU" device="0805" value="220R"/>
<part name="R23" library="_newbylars" deviceset="R-EU" device="0805" value="220R"/>
<part name="GND4" library="_newbylars" deviceset="GND" device=""/>
<part name="VCC11" library="supply" deviceset="VCC" device=""/>
<part name="TP1" library="_newbylars" deviceset="TP" device="TH-15-06"/>
<part name="TP2" library="_newbylars" deviceset="TP" device="TH-15-06"/>
<part name="TP3" library="_newbylars" deviceset="TP" device="TH-15-06"/>
<part name="TP4" library="_newbylars" deviceset="TP" device="TH-15-06"/>
<part name="TP5" library="_newbylars" deviceset="TP" device="TH-15-06"/>
<part name="TP6" library="_newbylars" deviceset="TP" device="TH-15-06"/>
<part name="C10" library="cap" deviceset="C-EU_" device="0805R" value="100nF"/>
<part name="C11" library="cap" deviceset="C-EU_" device="0805R" value="100nF"/>
<part name="R5" library="_newbylars" deviceset="R-TRIMM" device="" value="10K"/>
<part name="D3" library="_newbylars" deviceset="1N4148" device="MINIMELF" value="1N4148"/>
<part name="D4" library="_newbylars" deviceset="1N4148" device="MINIMELF" value="1N4148"/>
<part name="D5" library="_newbylars" deviceset="SUPPRESSOR-" device="SMB"/>
<part name="D6" library="_newbylars" deviceset="SUPPRESSOR-" device="SMB"/>
<part name="D7" library="_newbylars" deviceset="SUPPRESSOR-" device="SMB"/>
<part name="R14" library="_newbylars" deviceset="R-EU" device="LEADED-7.5" value="100"/>
<part name="HOLE1" library="_newbylars" deviceset="HOLE_PAD" device=""/>
<part name="HOLE2" library="_newbylars" deviceset="HOLE_PAD" device=""/>
<part name="HOLE3" library="_newbylars" deviceset="HOLE_PAD" device=""/>
<part name="HOLE4" library="_newbylars" deviceset="HOLE_PAD" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="241.3" y="132.08" size="1.4224" layer="91">D-</text>
<text x="241.3" y="134.62" size="1.4224" layer="91">+5V</text>
<text x="241.3" y="129.54" size="1.4224" layer="91">D+</text>
</plain>
<instances>
<instance part="U1" gate="G$1" x="152.4" y="109.22"/>
<instance part="FRAME1" gate="G$1" x="0" y="0"/>
<instance part="FRAME1" gate="G$2" x="162.56" y="0"/>
<instance part="J4" gate="G$1" x="22.86" y="58.42"/>
<instance part="J3" gate="G$1" x="22.86" y="25.4"/>
<instance part="J2" gate="G$1" x="22.86" y="91.44"/>
<instance part="J5" gate="G$1" x="246.38" y="96.52" rot="R180"/>
<instance part="J9" gate="G$1" x="144.78" y="165.1" rot="R180"/>
<instance part="J1" gate="G$1" x="248.92" y="129.54"/>
<instance part="C1" gate="G$1" x="248.92" y="157.48"/>
<instance part="C2" gate="G$1" x="106.68" y="116.84"/>
<instance part="VCC1" gate="VCC" x="248.92" y="165.1"/>
<instance part="J6" gate="G$1" x="129.54" y="38.1" rot="R180"/>
<instance part="R1" gate="G$1" x="205.74" y="147.32"/>
<instance part="OK1" gate="G$1" x="68.58" y="58.42"/>
<instance part="GND2" gate="1" x="238.76" y="116.84"/>
<instance part="GND1" gate="1" x="248.92" y="147.32"/>
<instance part="D1" gate="G$1" x="231.14" y="124.46" rot="R90"/>
<instance part="D2" gate="G$1" x="220.98" y="124.46" rot="R90"/>
<instance part="R2" gate="G$1" x="205.74" y="132.08"/>
<instance part="R3" gate="G$1" x="210.82" y="137.16" rot="R90"/>
<instance part="R4" gate="G$1" x="210.82" y="127" rot="R90"/>
<instance part="XT1" gate="G$1" x="114.3" y="134.62"/>
<instance part="C3" gate="G$1" x="116.84" y="116.84"/>
<instance part="C4" gate="G$1" x="104.14" y="139.7" rot="R90"/>
<instance part="C5" gate="G$1" x="104.14" y="129.54" rot="R90"/>
<instance part="GND6" gate="1" x="96.52" y="124.46"/>
<instance part="C6" gate="G$1" x="114.3" y="104.14"/>
<instance part="GND7" gate="1" x="114.3" y="96.52"/>
<instance part="GND8" gate="1" x="106.68" y="109.22"/>
<instance part="VCC2" gate="VCC" x="106.68" y="119.38"/>
<instance part="GND9" gate="1" x="162.56" y="157.48"/>
<instance part="VCC3" gate="VCC" x="162.56" y="167.64"/>
<instance part="GND10" gate="1" x="218.44" y="91.44"/>
<instance part="VCC4" gate="VCC" x="218.44" y="106.68"/>
<instance part="OK2" gate="G$1" x="68.58" y="25.4"/>
<instance part="GND11" gate="1" x="86.36" y="10.16"/>
<instance part="R6" gate="G$1" x="86.36" y="17.78" rot="R90"/>
<instance part="VCC5" gate="VCC" x="91.44" y="35.56"/>
<instance part="R7" gate="G$1" x="91.44" y="30.48" rot="R90"/>
<instance part="R8" gate="G$1" x="45.72" y="30.48" rot="R180"/>
<instance part="R9" gate="G$1" x="45.72" y="63.5" rot="R180"/>
<instance part="GND12" gate="1" x="86.36" y="43.18"/>
<instance part="R10" gate="G$1" x="86.36" y="50.8" rot="R90"/>
<instance part="R11" gate="G$1" x="91.44" y="63.5" rot="R90"/>
<instance part="R12" gate="G$1" x="45.72" y="83.82" rot="R90"/>
<instance part="R13" gate="G$1" x="45.72" y="93.98" rot="R180"/>
<instance part="VCC7" gate="VCC" x="91.44" y="68.58"/>
<instance part="VCC8" gate="VCC" x="53.34" y="96.52"/>
<instance part="Q1" gate="G$1" x="226.06" y="50.8"/>
<instance part="GND13" gate="1" x="226.06" y="43.18"/>
<instance part="VCC6" gate="VCC" x="226.06" y="68.58"/>
<instance part="C7" gate="G$1" x="226.06" y="99.06"/>
<instance part="GND14" gate="1" x="243.84" y="101.6"/>
<instance part="GND15" gate="1" x="114.3" y="20.32"/>
<instance part="J8" gate="G$1" x="25.4" y="152.4"/>
<instance part="J7" gate="G$1" x="25.4" y="121.92"/>
<instance part="R15" gate="G$1" x="50.8" y="160.02" rot="R90"/>
<instance part="R16" gate="G$1" x="60.96" y="160.02" rot="R90"/>
<instance part="R17" gate="G$1" x="43.18" y="154.94"/>
<instance part="C8" gate="G$1" x="60.96" y="149.86"/>
<instance part="GND16" gate="1" x="60.96" y="139.7"/>
<instance part="R18" gate="G$1" x="48.26" y="129.54" rot="R90"/>
<instance part="R19" gate="G$1" x="58.42" y="129.54" rot="R90"/>
<instance part="R20" gate="G$1" x="40.64" y="124.46"/>
<instance part="C9" gate="G$1" x="58.42" y="119.38"/>
<instance part="GND17" gate="1" x="58.42" y="109.22"/>
<instance part="VCC10" gate="VCC" x="48.26" y="134.62"/>
<instance part="VCC9" gate="VCC" x="50.8" y="165.1"/>
<instance part="CGND1" gate="G$1" x="246.38" y="116.84"/>
<instance part="CGND2" gate="G$1" x="38.1" y="73.66"/>
<instance part="CGND3" gate="G$1" x="38.1" y="40.64"/>
<instance part="CGND4" gate="G$1" x="38.1" y="7.62"/>
<instance part="GND3" gate="1" x="86.36" y="160.02"/>
<instance part="CGND5" gate="G$1" x="101.6" y="162.56"/>
<instance part="R21" gate="G$1" x="93.98" y="165.1" rot="R180"/>
<instance part="D9" gate="G$1" x="215.9" y="157.48" smashed="yes" rot="R270">
<attribute name="NAME" x="213.868" y="152.8826" size="1.778" layer="95" font="vector"/>
<attribute name="VALUE" x="213.868" y="150.0886" size="1.778" layer="96" font="vector"/>
</instance>
<instance part="D8" gate="G$1" x="215.9" y="167.64" smashed="yes" rot="R270">
<attribute name="NAME" x="213.868" y="163.0426" size="1.778" layer="95" font="vector"/>
<attribute name="VALUE" x="213.868" y="160.2486" size="1.778" layer="96" font="vector"/>
</instance>
<instance part="R22" gate="G$1" x="205.74" y="157.48" rot="R180"/>
<instance part="R23" gate="G$1" x="205.74" y="167.64" rot="R180"/>
<instance part="GND4" gate="1" x="198.12" y="162.56"/>
<instance part="VCC11" gate="VCC" x="223.52" y="167.64"/>
<instance part="TP1" gate="G$1" x="86.36" y="170.18"/>
<instance part="TP2" gate="G$1" x="241.3" y="167.64"/>
<instance part="TP3" gate="G$1" x="101.6" y="63.5" smashed="yes">
<attribute name="NAME" x="100.076" y="64.516" size="1.778" layer="95"/>
</instance>
<instance part="TP4" gate="G$1" x="101.6" y="30.48" smashed="yes">
<attribute name="NAME" x="99.568" y="31.496" size="1.778" layer="95"/>
</instance>
<instance part="TP5" gate="G$1" x="71.12" y="160.02"/>
<instance part="TP6" gate="G$1" x="68.58" y="129.54"/>
<instance part="C10" gate="G$1" x="96.52" y="17.78"/>
<instance part="C11" gate="G$1" x="96.52" y="50.8"/>
<instance part="R5" gate="G$1" x="218.44" y="101.6"/>
<instance part="D3" gate="G$1" x="53.34" y="58.42" smashed="yes" rot="R90">
<attribute name="NAME" x="50.8" y="57.9374" size="1.778" layer="95" font="vector" rot="R180"/>
<attribute name="VALUE" x="53.34" y="55.6514" size="1.778" layer="96" font="vector" rot="R180"/>
</instance>
<instance part="D4" gate="G$1" x="53.34" y="25.4" smashed="yes" rot="R90">
<attribute name="NAME" x="50.8" y="24.9174" size="1.778" layer="95" font="vector" rot="R180"/>
<attribute name="VALUE" x="53.34" y="22.6314" size="1.778" layer="96" font="vector" rot="R180"/>
</instance>
<instance part="D5" gate="G$1" x="33.02" y="119.38" smashed="yes">
<attribute name="NAME" x="34.798" y="117.221" size="1.778" layer="95"/>
<attribute name="VALUE" x="35.814" y="118.491" size="1.778" layer="96"/>
</instance>
<instance part="D6" gate="G$1" x="33.02" y="149.86" smashed="yes">
<attribute name="NAME" x="34.798" y="148.971" size="1.778" layer="95"/>
<attribute name="VALUE" x="35.814" y="148.971" size="1.778" layer="96"/>
</instance>
<instance part="D7" gate="G$1" x="241.3" y="157.48" smashed="yes">
<attribute name="NAME" x="242.062" y="153.543" size="1.778" layer="95"/>
<attribute name="VALUE" x="244.094" y="156.591" size="1.778" layer="96"/>
</instance>
<instance part="R14" gate="G$1" x="226.06" y="63.5" rot="R90"/>
<instance part="HOLE1" gate="G$1" x="101.6" y="170.18"/>
<instance part="HOLE2" gate="G$1" x="109.22" y="170.18"/>
<instance part="HOLE3" gate="G$1" x="116.84" y="170.18"/>
<instance part="HOLE4" gate="G$1" x="124.46" y="170.18"/>
</instances>
<busses>
<bus name="BIT[0..7],RS,R/W,CLK">
<segment>
<wire x1="185.42" y1="116.84" x2="185.42" y2="86.36" width="0.762" layer="92"/>
<wire x1="236.22" y1="88.9" x2="236.22" y2="86.36" width="0.762" layer="92"/>
<wire x1="236.22" y1="86.36" x2="236.22" y2="60.96" width="0.762" layer="92"/>
<wire x1="185.42" y1="86.36" x2="236.22" y2="86.36" width="0.762" layer="92"/>
</segment>
</bus>
<bus name="GC1,GC2,PUSH,SEL,BACK">
<segment>
<wire x1="119.38" y1="93.98" x2="119.38" y2="25.4" width="0.762" layer="92"/>
<wire x1="119.38" y1="25.4" x2="154.94" y2="25.4" width="0.762" layer="92"/>
<wire x1="154.94" y1="25.4" x2="154.94" y2="40.64" width="0.762" layer="92"/>
</segment>
</bus>
</busses>
<nets>
<net name="VCC" class="0">
<segment>
<wire x1="106.68" y1="119.38" x2="116.84" y2="119.38" width="0.1524" layer="91"/>
<wire x1="116.84" y1="119.38" x2="121.92" y2="119.38" width="0.1524" layer="91"/>
<wire x1="121.92" y1="119.38" x2="127" y2="119.38" width="0.1524" layer="91"/>
<wire x1="127" y1="119.38" x2="129.54" y2="119.38" width="0.1524" layer="91"/>
<wire x1="127" y1="119.38" x2="127" y2="121.92" width="0.1524" layer="91"/>
<wire x1="127" y1="121.92" x2="129.54" y2="121.92" width="0.1524" layer="91"/>
<wire x1="127" y1="121.92" x2="127" y2="124.46" width="0.1524" layer="91"/>
<wire x1="127" y1="124.46" x2="129.54" y2="124.46" width="0.1524" layer="91"/>
<wire x1="129.54" y1="106.68" x2="121.92" y2="106.68" width="0.1524" layer="91"/>
<wire x1="121.92" y1="106.68" x2="121.92" y2="119.38" width="0.1524" layer="91"/>
<junction x="106.68" y="119.38"/>
<junction x="116.84" y="119.38"/>
<junction x="127" y="119.38"/>
<junction x="127" y="121.92"/>
<junction x="121.92" y="119.38"/>
<pinref part="C2" gate="G$1" pin="1"/>
<pinref part="VCC2" gate="VCC" pin="VCC"/>
<pinref part="C3" gate="G$1" pin="1"/>
<pinref part="U1" gate="G$1" pin="VCC@2"/>
<pinref part="U1" gate="G$1" pin="VCC@1"/>
<pinref part="U1" gate="G$1" pin="VCC@3"/>
<pinref part="U1" gate="G$1" pin="AVCC"/>
</segment>
<segment>
<wire x1="160.02" y1="165.1" x2="162.56" y2="165.1" width="0.1524" layer="91"/>
<wire x1="162.56" y1="165.1" x2="162.56" y2="167.64" width="0.1524" layer="91"/>
<pinref part="J9" gate="G$1" pin="2"/>
<pinref part="VCC3" gate="VCC" pin="VCC"/>
</segment>
<segment>
<wire x1="91.44" y1="35.56" x2="86.36" y2="35.56" width="0.1524" layer="91"/>
<wire x1="96.52" y1="20.32" x2="96.52" y2="35.56" width="0.1524" layer="91"/>
<wire x1="96.52" y1="35.56" x2="91.44" y2="35.56" width="0.1524" layer="91"/>
<wire x1="86.36" y1="35.56" x2="86.36" y2="30.48" width="0.1524" layer="91"/>
<wire x1="86.36" y1="30.48" x2="81.28" y2="30.48" width="0.1524" layer="91"/>
<junction x="91.44" y="35.56"/>
<pinref part="VCC5" gate="VCC" pin="VCC"/>
<pinref part="R7" gate="G$1" pin="2"/>
<pinref part="C10" gate="G$1" pin="1"/>
<pinref part="OK2" gate="G$1" pin="VCC"/>
</segment>
<segment>
<wire x1="50.8" y1="93.98" x2="53.34" y2="93.98" width="0.1524" layer="91"/>
<wire x1="53.34" y1="93.98" x2="53.34" y2="96.52" width="0.1524" layer="91"/>
<pinref part="R13" gate="G$1" pin="1"/>
<pinref part="VCC8" gate="VCC" pin="VCC"/>
</segment>
<segment>
<wire x1="86.36" y1="68.58" x2="91.44" y2="68.58" width="0.1524" layer="91"/>
<wire x1="86.36" y1="63.5" x2="86.36" y2="68.58" width="0.1524" layer="91"/>
<wire x1="81.28" y1="63.5" x2="86.36" y2="63.5" width="0.1524" layer="91"/>
<wire x1="96.52" y1="53.34" x2="96.52" y2="68.58" width="0.1524" layer="91"/>
<wire x1="96.52" y1="68.58" x2="91.44" y2="68.58" width="0.1524" layer="91"/>
<junction x="91.44" y="68.58"/>
<pinref part="VCC7" gate="VCC" pin="VCC"/>
<pinref part="OK1" gate="G$1" pin="VCC"/>
<pinref part="R11" gate="G$1" pin="2"/>
<pinref part="C11" gate="G$1" pin="1"/>
</segment>
<segment>
<wire x1="246.38" y1="93.98" x2="236.22" y2="93.98" width="0.1524" layer="91"/>
<wire x1="236.22" y1="93.98" x2="236.22" y2="106.68" width="0.1524" layer="91"/>
<wire x1="218.44" y1="106.68" x2="236.22" y2="106.68" width="0.1524" layer="91"/>
<junction x="218.44" y="106.68"/>
<pinref part="VCC4" gate="VCC" pin="VCC"/>
<pinref part="J5" gate="G$1" pin="2"/>
<pinref part="R5" gate="G$1" pin="A"/>
</segment>
<segment>
<wire x1="48.26" y1="134.62" x2="58.42" y2="134.62" width="0.1524" layer="91"/>
<junction x="48.26" y="134.62"/>
<pinref part="R19" gate="G$1" pin="2"/>
<pinref part="R18" gate="G$1" pin="2"/>
<pinref part="VCC10" gate="VCC" pin="VCC"/>
</segment>
<segment>
<wire x1="60.96" y1="165.1" x2="50.8" y2="165.1" width="0.1524" layer="91"/>
<junction x="50.8" y="165.1"/>
<pinref part="R15" gate="G$1" pin="2"/>
<pinref part="VCC9" gate="VCC" pin="VCC"/>
<pinref part="R16" gate="G$1" pin="2"/>
</segment>
<segment>
<wire x1="223.52" y1="167.64" x2="218.44" y2="167.64" width="0.1524" layer="91"/>
<wire x1="223.52" y1="157.48" x2="223.52" y2="167.64" width="0.1524" layer="91"/>
<wire x1="218.44" y1="157.48" x2="223.52" y2="157.48" width="0.1524" layer="91"/>
<junction x="223.52" y="167.64"/>
<pinref part="D8" gate="G$1" pin="A"/>
<pinref part="D9" gate="G$1" pin="A"/>
<pinref part="VCC11" gate="VCC" pin="VCC"/>
</segment>
<segment>
<wire x1="236.22" y1="134.62" x2="246.38" y2="134.62" width="0.1524" layer="91"/>
<wire x1="236.22" y1="134.62" x2="236.22" y2="162.56" width="0.1524" layer="91"/>
<wire x1="236.22" y1="162.56" x2="241.3" y2="162.56" width="0.1524" layer="91"/>
<wire x1="241.3" y1="162.56" x2="248.92" y2="162.56" width="0.1524" layer="91"/>
<wire x1="241.3" y1="162.56" x2="241.3" y2="160.02" width="0.1524" layer="91"/>
<wire x1="248.92" y1="162.56" x2="248.92" y2="160.02" width="0.1524" layer="91"/>
<wire x1="241.3" y1="162.56" x2="241.3" y2="165.1" width="0.1524" layer="91"/>
<wire x1="248.92" y1="165.1" x2="248.92" y2="162.56" width="0.1524" layer="91"/>
<junction x="241.3" y="162.56"/>
<junction x="248.92" y="162.56"/>
<pinref part="J1" gate="G$1" pin="1"/>
<pinref part="D7" gate="G$1" pin="C"/>
<pinref part="C1" gate="G$1" pin="1"/>
<pinref part="TP2" gate="G$1" pin="TP"/>
<pinref part="VCC1" gate="VCC" pin="VCC"/>
</segment>
<segment>
<pinref part="VCC6" gate="VCC" pin="VCC"/>
<pinref part="R14" gate="G$1" pin="2"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<wire x1="246.38" y1="127" x2="238.76" y2="127" width="0.1524" layer="91"/>
<wire x1="238.76" y1="127" x2="238.76" y2="119.38" width="0.1524" layer="91"/>
<wire x1="210.82" y1="121.92" x2="210.82" y2="119.38" width="0.1524" layer="91"/>
<wire x1="210.82" y1="119.38" x2="220.98" y2="119.38" width="0.1524" layer="91"/>
<wire x1="220.98" y1="119.38" x2="231.14" y2="119.38" width="0.1524" layer="91"/>
<wire x1="231.14" y1="119.38" x2="238.76" y2="119.38" width="0.1524" layer="91"/>
<wire x1="220.98" y1="121.92" x2="220.98" y2="119.38" width="0.1524" layer="91"/>
<wire x1="231.14" y1="121.92" x2="231.14" y2="119.38" width="0.1524" layer="91"/>
<junction x="238.76" y="119.38"/>
<junction x="220.98" y="119.38"/>
<junction x="231.14" y="119.38"/>
<label x="241.3" y="127" size="1.4224" layer="95"/>
<pinref part="J1" gate="G$1" pin="4"/>
<pinref part="GND2" gate="1" pin="GND"/>
<pinref part="R4" gate="G$1" pin="1"/>
<pinref part="D2" gate="G$1" pin="A"/>
<pinref part="D1" gate="G$1" pin="A"/>
</segment>
<segment>
<wire x1="96.52" y1="127" x2="96.52" y2="129.54" width="0.1524" layer="91"/>
<wire x1="96.52" y1="129.54" x2="101.6" y2="129.54" width="0.1524" layer="91"/>
<wire x1="96.52" y1="129.54" x2="96.52" y2="139.7" width="0.1524" layer="91"/>
<wire x1="96.52" y1="139.7" x2="101.6" y2="139.7" width="0.1524" layer="91"/>
<junction x="96.52" y="129.54"/>
<pinref part="GND6" gate="1" pin="GND"/>
<pinref part="C5" gate="G$1" pin="1"/>
<pinref part="C4" gate="G$1" pin="1"/>
</segment>
<segment>
<wire x1="129.54" y1="99.06" x2="114.3" y2="99.06" width="0.1524" layer="91"/>
<junction x="114.3" y="99.06"/>
<pinref part="C6" gate="G$1" pin="2"/>
<pinref part="GND7" gate="1" pin="GND"/>
<pinref part="U1" gate="G$1" pin="AGND"/>
</segment>
<segment>
<wire x1="106.68" y1="111.76" x2="116.84" y2="111.76" width="0.1524" layer="91"/>
<wire x1="116.84" y1="111.76" x2="127" y2="111.76" width="0.1524" layer="91"/>
<wire x1="127" y1="111.76" x2="129.54" y2="111.76" width="0.1524" layer="91"/>
<wire x1="129.54" y1="114.3" x2="127" y2="114.3" width="0.1524" layer="91"/>
<wire x1="127" y1="114.3" x2="127" y2="111.76" width="0.1524" layer="91"/>
<wire x1="129.54" y1="116.84" x2="127" y2="116.84" width="0.1524" layer="91"/>
<wire x1="127" y1="116.84" x2="127" y2="114.3" width="0.1524" layer="91"/>
<junction x="106.68" y="111.76"/>
<junction x="116.84" y="111.76"/>
<junction x="127" y="111.76"/>
<junction x="127" y="114.3"/>
<pinref part="C2" gate="G$1" pin="2"/>
<pinref part="GND8" gate="1" pin="GND"/>
<pinref part="C3" gate="G$1" pin="2"/>
<pinref part="U1" gate="G$1" pin="GND@2"/>
<pinref part="U1" gate="G$1" pin="GND@1"/>
<pinref part="U1" gate="G$1" pin="GND@3"/>
</segment>
<segment>
<wire x1="160.02" y1="160.02" x2="162.56" y2="160.02" width="0.1524" layer="91"/>
<pinref part="J9" gate="G$1" pin="6"/>
<pinref part="GND9" gate="1" pin="GND"/>
</segment>
<segment>
<wire x1="81.28" y1="20.32" x2="81.28" y2="12.7" width="0.1524" layer="91"/>
<wire x1="81.28" y1="12.7" x2="86.36" y2="12.7" width="0.1524" layer="91"/>
<wire x1="86.36" y1="12.7" x2="96.52" y2="12.7" width="0.1524" layer="91"/>
<junction x="86.36" y="12.7"/>
<pinref part="GND11" gate="1" pin="GND"/>
<pinref part="R6" gate="G$1" pin="1"/>
<pinref part="OK2" gate="G$1" pin="GND"/>
<pinref part="C10" gate="G$1" pin="2"/>
</segment>
<segment>
<wire x1="81.28" y1="53.34" x2="81.28" y2="45.72" width="0.1524" layer="91"/>
<wire x1="81.28" y1="45.72" x2="86.36" y2="45.72" width="0.1524" layer="91"/>
<wire x1="86.36" y1="45.72" x2="96.52" y2="45.72" width="0.1524" layer="91"/>
<junction x="86.36" y="45.72"/>
<pinref part="GND12" gate="1" pin="GND"/>
<pinref part="R10" gate="G$1" pin="1"/>
<pinref part="OK1" gate="G$1" pin="GND"/>
<pinref part="C11" gate="G$1" pin="2"/>
</segment>
<segment>
<pinref part="Q1" gate="G$1" pin="S"/>
<pinref part="GND13" gate="1" pin="GND"/>
</segment>
<segment>
<wire x1="218.44" y1="96.52" x2="218.44" y2="93.98" width="0.1524" layer="91"/>
<wire x1="226.06" y1="93.98" x2="218.44" y2="93.98" width="0.1524" layer="91"/>
<junction x="218.44" y="93.98"/>
<pinref part="GND10" gate="1" pin="GND"/>
<pinref part="C7" gate="G$1" pin="2"/>
<pinref part="R5" gate="G$1" pin="E"/>
</segment>
<segment>
<wire x1="243.84" y1="104.14" x2="238.76" y2="104.14" width="0.1524" layer="91"/>
<wire x1="238.76" y1="104.14" x2="238.76" y2="96.52" width="0.1524" layer="91"/>
<wire x1="238.76" y1="96.52" x2="246.38" y2="96.52" width="0.1524" layer="91"/>
<pinref part="GND14" gate="1" pin="GND"/>
<pinref part="J5" gate="G$1" pin="1"/>
</segment>
<segment>
<wire x1="58.42" y1="114.3" x2="58.42" y2="111.76" width="0.1524" layer="91"/>
<wire x1="58.42" y1="114.3" x2="33.02" y2="114.3" width="0.1524" layer="91"/>
<wire x1="33.02" y1="114.3" x2="27.94" y2="114.3" width="0.1524" layer="91"/>
<wire x1="27.94" y1="114.3" x2="27.94" y2="121.92" width="0.1524" layer="91"/>
<wire x1="27.94" y1="121.92" x2="25.4" y2="121.92" width="0.1524" layer="91"/>
<wire x1="33.02" y1="116.84" x2="33.02" y2="114.3" width="0.1524" layer="91"/>
<junction x="58.42" y="114.3"/>
<junction x="33.02" y="114.3"/>
<pinref part="C9" gate="G$1" pin="2"/>
<pinref part="GND17" gate="1" pin="GND"/>
<pinref part="J7" gate="G$1" pin="1"/>
<pinref part="D5" gate="G$1" pin="A"/>
</segment>
<segment>
<wire x1="88.9" y1="165.1" x2="86.36" y2="165.1" width="0.1524" layer="91"/>
<wire x1="86.36" y1="165.1" x2="86.36" y2="162.56" width="0.1524" layer="91"/>
<wire x1="86.36" y1="165.1" x2="86.36" y2="167.64" width="0.1524" layer="91"/>
<junction x="86.36" y="165.1"/>
<pinref part="R21" gate="G$1" pin="2"/>
<pinref part="GND3" gate="1" pin="GND"/>
<pinref part="TP1" gate="G$1" pin="TP"/>
</segment>
<segment>
<wire x1="200.66" y1="167.64" x2="198.12" y2="167.64" width="0.1524" layer="91"/>
<wire x1="198.12" y1="167.64" x2="198.12" y2="165.1" width="0.1524" layer="91"/>
<pinref part="R23" gate="G$1" pin="2"/>
<pinref part="GND4" gate="1" pin="GND"/>
</segment>
<segment>
<wire x1="60.96" y1="144.78" x2="60.96" y2="142.24" width="0.1524" layer="91"/>
<wire x1="60.96" y1="144.78" x2="33.02" y2="144.78" width="0.1524" layer="91"/>
<wire x1="33.02" y1="144.78" x2="27.94" y2="144.78" width="0.1524" layer="91"/>
<wire x1="27.94" y1="144.78" x2="27.94" y2="152.4" width="0.1524" layer="91"/>
<wire x1="27.94" y1="152.4" x2="25.4" y2="152.4" width="0.1524" layer="91"/>
<wire x1="33.02" y1="147.32" x2="33.02" y2="144.78" width="0.1524" layer="91"/>
<junction x="60.96" y="144.78"/>
<junction x="33.02" y="144.78"/>
<pinref part="C8" gate="G$1" pin="2"/>
<pinref part="GND16" gate="1" pin="GND"/>
<pinref part="J8" gate="G$1" pin="1"/>
<pinref part="D6" gate="G$1" pin="A"/>
</segment>
<segment>
<wire x1="129.54" y1="38.1" x2="114.3" y2="38.1" width="0.1524" layer="91"/>
<wire x1="114.3" y1="38.1" x2="114.3" y2="22.86" width="0.1524" layer="91"/>
<pinref part="J6" gate="G$1" pin="1"/>
<pinref part="GND15" gate="1" pin="GND"/>
</segment>
<segment>
<wire x1="248.92" y1="152.4" x2="241.3" y2="152.4" width="0.1524" layer="91"/>
<wire x1="241.3" y1="152.4" x2="241.3" y2="154.94" width="0.1524" layer="91"/>
<wire x1="248.92" y1="149.86" x2="248.92" y2="152.4" width="0.1524" layer="91"/>
<junction x="248.92" y="152.4"/>
<pinref part="C1" gate="G$1" pin="2"/>
<pinref part="D7" gate="G$1" pin="A"/>
<pinref part="GND1" gate="1" pin="GND"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<wire x1="231.14" y1="127" x2="231.14" y2="129.54" width="0.1524" layer="91"/>
<wire x1="231.14" y1="129.54" x2="246.38" y2="129.54" width="0.1524" layer="91"/>
<wire x1="231.14" y1="129.54" x2="231.14" y2="147.32" width="0.1524" layer="91"/>
<wire x1="231.14" y1="147.32" x2="210.82" y2="147.32" width="0.1524" layer="91"/>
<junction x="231.14" y="129.54"/>
<pinref part="D1" gate="G$1" pin="C"/>
<pinref part="J1" gate="G$1" pin="3"/>
<pinref part="R1" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<wire x1="246.38" y1="132.08" x2="220.98" y2="132.08" width="0.1524" layer="91"/>
<wire x1="220.98" y1="132.08" x2="220.98" y2="127" width="0.1524" layer="91"/>
<wire x1="220.98" y1="132.08" x2="210.82" y2="132.08" width="0.1524" layer="91"/>
<junction x="220.98" y="132.08"/>
<junction x="210.82" y="132.08"/>
<pinref part="J1" gate="G$1" pin="2"/>
<pinref part="D2" gate="G$1" pin="C"/>
<pinref part="R2" gate="G$1" pin="2"/>
<pinref part="R4" gate="G$1" pin="2"/>
<pinref part="R3" gate="G$1" pin="1"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<wire x1="175.26" y1="121.92" x2="200.66" y2="121.92" width="0.1524" layer="91"/>
<wire x1="200.66" y1="121.92" x2="200.66" y2="132.08" width="0.1524" layer="91"/>
<pinref part="U1" gate="G$1" pin="PB0(T0/XCK)"/>
<pinref part="R2" gate="G$1" pin="1"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<wire x1="175.26" y1="124.46" x2="198.12" y2="124.46" width="0.1524" layer="91"/>
<wire x1="198.12" y1="124.46" x2="198.12" y2="142.24" width="0.1524" layer="91"/>
<wire x1="198.12" y1="142.24" x2="210.82" y2="142.24" width="0.1524" layer="91"/>
<pinref part="U1" gate="G$1" pin="PB1(T1/CLKO)"/>
<pinref part="R3" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<wire x1="200.66" y1="147.32" x2="195.58" y2="147.32" width="0.1524" layer="91"/>
<wire x1="195.58" y1="147.32" x2="195.58" y2="127" width="0.1524" layer="91"/>
<wire x1="195.58" y1="127" x2="175.26" y2="127" width="0.1524" layer="91"/>
<pinref part="R1" gate="G$1" pin="1"/>
<pinref part="U1" gate="G$1" pin="PB2(AIN0/INT2)"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<wire x1="129.54" y1="129.54" x2="114.3" y2="129.54" width="0.1524" layer="91"/>
<wire x1="114.3" y1="129.54" x2="109.22" y2="129.54" width="0.1524" layer="91"/>
<wire x1="114.3" y1="132.08" x2="114.3" y2="129.54" width="0.1524" layer="91"/>
<junction x="114.3" y="129.54"/>
<pinref part="U1" gate="G$1" pin="XTAL1"/>
<pinref part="C5" gate="G$1" pin="2"/>
<pinref part="XT1" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<wire x1="109.22" y1="139.7" x2="114.3" y2="139.7" width="0.1524" layer="91"/>
<wire x1="114.3" y1="139.7" x2="114.3" y2="137.16" width="0.1524" layer="91"/>
<wire x1="129.54" y1="134.62" x2="121.92" y2="134.62" width="0.1524" layer="91"/>
<wire x1="121.92" y1="134.62" x2="121.92" y2="139.7" width="0.1524" layer="91"/>
<wire x1="121.92" y1="139.7" x2="114.3" y2="139.7" width="0.1524" layer="91"/>
<junction x="114.3" y="139.7"/>
<pinref part="C4" gate="G$1" pin="2"/>
<pinref part="XT1" gate="G$1" pin="1"/>
<pinref part="U1" gate="G$1" pin="XTAL2"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<wire x1="129.54" y1="139.7" x2="129.54" y2="160.02" width="0.1524" layer="91"/>
<wire x1="129.54" y1="160.02" x2="144.78" y2="160.02" width="0.1524" layer="91"/>
<pinref part="U1" gate="G$1" pin="/RESET"/>
<pinref part="J9" gate="G$1" pin="5"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<wire x1="144.78" y1="165.1" x2="139.7" y2="165.1" width="0.1524" layer="91"/>
<wire x1="139.7" y1="165.1" x2="139.7" y2="149.86" width="0.1524" layer="91"/>
<wire x1="139.7" y1="149.86" x2="180.34" y2="149.86" width="0.1524" layer="91"/>
<wire x1="180.34" y1="149.86" x2="180.34" y2="137.16" width="0.1524" layer="91"/>
<wire x1="180.34" y1="137.16" x2="175.26" y2="137.16" width="0.1524" layer="91"/>
<pinref part="J9" gate="G$1" pin="1"/>
<pinref part="U1" gate="G$1" pin="PB6(MISO)"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<wire x1="144.78" y1="162.56" x2="142.24" y2="162.56" width="0.1524" layer="91"/>
<wire x1="142.24" y1="162.56" x2="142.24" y2="152.4" width="0.1524" layer="91"/>
<wire x1="142.24" y1="152.4" x2="177.8" y2="152.4" width="0.1524" layer="91"/>
<wire x1="177.8" y1="152.4" x2="177.8" y2="139.7" width="0.1524" layer="91"/>
<wire x1="177.8" y1="139.7" x2="175.26" y2="139.7" width="0.1524" layer="91"/>
<pinref part="J9" gate="G$1" pin="3"/>
<pinref part="U1" gate="G$1" pin="PB7(SCK)"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<wire x1="160.02" y1="162.56" x2="182.88" y2="162.56" width="0.1524" layer="91"/>
<wire x1="182.88" y1="162.56" x2="182.88" y2="134.62" width="0.1524" layer="91"/>
<wire x1="182.88" y1="134.62" x2="175.26" y2="134.62" width="0.1524" layer="91"/>
<pinref part="J9" gate="G$1" pin="4"/>
<pinref part="U1" gate="G$1" pin="PB5(MOSI)"/>
</segment>
</net>
<net name="BIT7" class="0">
<segment>
<wire x1="175.26" y1="116.84" x2="182.88" y2="116.84" width="0.1524" layer="91"/>
<wire x1="182.88" y1="116.84" x2="185.42" y2="114.3" width="0.1524" layer="91"/>
<label x="177.8" y="116.84" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PC7(TOSC2)"/>
</segment>
<segment>
<wire x1="246.38" y1="63.5" x2="238.76" y2="63.5" width="0.1524" layer="91"/>
<wire x1="238.76" y1="63.5" x2="236.22" y2="60.96" width="0.1524" layer="91"/>
<label x="241.3" y="63.5" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="14"/>
</segment>
</net>
<net name="BIT6" class="0">
<segment>
<wire x1="175.26" y1="114.3" x2="182.88" y2="114.3" width="0.1524" layer="91"/>
<wire x1="182.88" y1="114.3" x2="185.42" y2="111.76" width="0.1524" layer="91"/>
<label x="177.8" y="114.3" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PC6(TOSC1)"/>
</segment>
<segment>
<wire x1="246.38" y1="66.04" x2="238.76" y2="66.04" width="0.1524" layer="91"/>
<wire x1="238.76" y1="66.04" x2="236.22" y2="63.5" width="0.1524" layer="91"/>
<label x="241.3" y="66.04" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="13"/>
</segment>
</net>
<net name="BIT5" class="0">
<segment>
<wire x1="175.26" y1="111.76" x2="182.88" y2="111.76" width="0.1524" layer="91"/>
<wire x1="182.88" y1="111.76" x2="185.42" y2="109.22" width="0.1524" layer="91"/>
<label x="177.8" y="111.76" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PC5(TDI)"/>
</segment>
<segment>
<wire x1="246.38" y1="68.58" x2="238.76" y2="68.58" width="0.1524" layer="91"/>
<wire x1="238.76" y1="68.58" x2="236.22" y2="66.04" width="0.1524" layer="91"/>
<label x="241.3" y="68.58" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="12"/>
</segment>
</net>
<net name="BIT4" class="0">
<segment>
<wire x1="175.26" y1="109.22" x2="182.88" y2="109.22" width="0.1524" layer="91"/>
<wire x1="182.88" y1="109.22" x2="185.42" y2="106.68" width="0.1524" layer="91"/>
<label x="177.8" y="109.22" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PC4(TDO)"/>
</segment>
<segment>
<wire x1="246.38" y1="71.12" x2="238.76" y2="71.12" width="0.1524" layer="91"/>
<wire x1="238.76" y1="71.12" x2="236.22" y2="68.58" width="0.1524" layer="91"/>
<label x="241.3" y="71.12" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="11"/>
</segment>
</net>
<net name="BIT3" class="0">
<segment>
<wire x1="175.26" y1="106.68" x2="182.88" y2="106.68" width="0.1524" layer="91"/>
<wire x1="182.88" y1="106.68" x2="185.42" y2="104.14" width="0.1524" layer="91"/>
<label x="177.8" y="106.68" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PC3(TMS)"/>
</segment>
<segment>
<wire x1="246.38" y1="73.66" x2="238.76" y2="73.66" width="0.1524" layer="91"/>
<wire x1="238.76" y1="73.66" x2="236.22" y2="71.12" width="0.1524" layer="91"/>
<label x="241.3" y="73.66" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="10"/>
</segment>
</net>
<net name="BIT2" class="0">
<segment>
<wire x1="175.26" y1="104.14" x2="182.88" y2="104.14" width="0.1524" layer="91"/>
<wire x1="182.88" y1="104.14" x2="185.42" y2="101.6" width="0.1524" layer="91"/>
<label x="177.8" y="104.14" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PC2(TCK)"/>
</segment>
<segment>
<wire x1="246.38" y1="76.2" x2="238.76" y2="76.2" width="0.1524" layer="91"/>
<wire x1="238.76" y1="76.2" x2="236.22" y2="73.66" width="0.1524" layer="91"/>
<label x="241.3" y="76.2" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="9"/>
</segment>
</net>
<net name="BIT1" class="0">
<segment>
<wire x1="175.26" y1="101.6" x2="182.88" y2="101.6" width="0.1524" layer="91"/>
<wire x1="182.88" y1="101.6" x2="185.42" y2="99.06" width="0.1524" layer="91"/>
<label x="177.8" y="101.6" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PC1(SDA)"/>
</segment>
<segment>
<wire x1="246.38" y1="78.74" x2="238.76" y2="78.74" width="0.1524" layer="91"/>
<wire x1="238.76" y1="78.74" x2="236.22" y2="76.2" width="0.1524" layer="91"/>
<label x="241.3" y="78.74" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="8"/>
</segment>
</net>
<net name="BIT0" class="0">
<segment>
<wire x1="175.26" y1="99.06" x2="182.88" y2="99.06" width="0.1524" layer="91"/>
<wire x1="182.88" y1="99.06" x2="185.42" y2="96.52" width="0.1524" layer="91"/>
<label x="177.8" y="99.06" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PC0(SCL)"/>
</segment>
<segment>
<wire x1="246.38" y1="81.28" x2="238.76" y2="81.28" width="0.1524" layer="91"/>
<wire x1="238.76" y1="81.28" x2="236.22" y2="78.74" width="0.1524" layer="91"/>
<label x="241.3" y="81.28" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="7"/>
</segment>
</net>
<net name="RS" class="0">
<segment>
<wire x1="246.38" y1="88.9" x2="238.76" y2="88.9" width="0.1524" layer="91"/>
<wire x1="238.76" y1="88.9" x2="236.22" y2="86.36" width="0.1524" layer="91"/>
<label x="241.3" y="88.9" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="4"/>
</segment>
<segment>
<wire x1="175.26" y1="88.9" x2="182.88" y2="88.9" width="0.1524" layer="91"/>
<wire x1="182.88" y1="88.9" x2="185.42" y2="86.36" width="0.1524" layer="91"/>
<label x="177.8" y="88.9" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PD5(OC1A)"/>
</segment>
</net>
<net name="R/W" class="0">
<segment>
<wire x1="246.38" y1="86.36" x2="238.76" y2="86.36" width="0.1524" layer="91"/>
<wire x1="238.76" y1="86.36" x2="236.22" y2="83.82" width="0.1524" layer="91"/>
<label x="241.3" y="86.36" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="5"/>
</segment>
<segment>
<wire x1="175.26" y1="91.44" x2="182.88" y2="91.44" width="0.1524" layer="91"/>
<wire x1="182.88" y1="91.44" x2="185.42" y2="88.9" width="0.1524" layer="91"/>
<label x="177.8" y="91.44" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PD6(ICP/OC2B)"/>
</segment>
</net>
<net name="CLK" class="0">
<segment>
<wire x1="246.38" y1="83.82" x2="238.76" y2="83.82" width="0.1524" layer="91"/>
<wire x1="238.76" y1="83.82" x2="236.22" y2="81.28" width="0.1524" layer="91"/>
<label x="241.3" y="83.82" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="6"/>
</segment>
<segment>
<wire x1="175.26" y1="93.98" x2="182.88" y2="93.98" width="0.1524" layer="91"/>
<wire x1="182.88" y1="93.98" x2="185.42" y2="91.44" width="0.1524" layer="91"/>
<label x="177.8" y="93.98" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PD7(OC2A)"/>
</segment>
</net>
<net name="CONTRAST" class="0">
<segment>
<wire x1="246.38" y1="91.44" x2="233.68" y2="91.44" width="0.1524" layer="91"/>
<wire x1="233.68" y1="91.44" x2="233.68" y2="101.6" width="0.1524" layer="91"/>
<wire x1="226.06" y1="101.6" x2="223.52" y2="101.6" width="0.1524" layer="91"/>
<wire x1="233.68" y1="101.6" x2="226.06" y2="101.6" width="0.1524" layer="91"/>
<junction x="226.06" y="101.6"/>
<label x="236.22" y="91.44" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="3"/>
<pinref part="C7" gate="G$1" pin="1"/>
<pinref part="R5" gate="G$1" pin="S"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<wire x1="81.28" y1="27.94" x2="86.36" y2="27.94" width="0.1524" layer="91"/>
<wire x1="86.36" y1="27.94" x2="86.36" y2="22.86" width="0.1524" layer="91"/>
<pinref part="OK2" gate="G$1" pin="VB"/>
<pinref part="R6" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<wire x1="38.1" y1="27.94" x2="40.64" y2="27.94" width="0.1524" layer="91"/>
<wire x1="40.64" y1="27.94" x2="40.64" y2="30.48" width="0.1524" layer="91"/>
<pinref part="J3" gate="G$1" pin="4"/>
<pinref part="R8" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$19" class="0">
<segment>
<wire x1="38.1" y1="22.86" x2="40.64" y2="22.86" width="0.1524" layer="91"/>
<wire x1="40.64" y1="22.86" x2="40.64" y2="20.32" width="0.1524" layer="91"/>
<wire x1="40.64" y1="20.32" x2="53.34" y2="20.32" width="0.1524" layer="91"/>
<wire x1="53.34" y1="20.32" x2="55.88" y2="20.32" width="0.1524" layer="91"/>
<wire x1="53.34" y1="20.32" x2="53.34" y2="22.86" width="0.1524" layer="91"/>
<junction x="53.34" y="20.32"/>
<pinref part="J3" gate="G$1" pin="5"/>
<pinref part="OK2" gate="G$1" pin="C"/>
<pinref part="D4" gate="G$1" pin="A"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<wire x1="50.8" y1="30.48" x2="53.34" y2="30.48" width="0.1524" layer="91"/>
<wire x1="53.34" y1="30.48" x2="55.88" y2="30.48" width="0.1524" layer="91"/>
<wire x1="53.34" y1="27.94" x2="53.34" y2="30.48" width="0.1524" layer="91"/>
<junction x="53.34" y="30.48"/>
<pinref part="R8" gate="G$1" pin="1"/>
<pinref part="OK2" gate="G$1" pin="A"/>
<pinref part="D4" gate="G$1" pin="C"/>
</segment>
</net>
<net name="N$20" class="0">
<segment>
<wire x1="86.36" y1="55.88" x2="86.36" y2="60.96" width="0.1524" layer="91"/>
<wire x1="86.36" y1="60.96" x2="81.28" y2="60.96" width="0.1524" layer="91"/>
<pinref part="R10" gate="G$1" pin="2"/>
<pinref part="OK1" gate="G$1" pin="VB"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<wire x1="55.88" y1="53.34" x2="53.34" y2="53.34" width="0.1524" layer="91"/>
<wire x1="53.34" y1="53.34" x2="53.34" y2="55.88" width="0.1524" layer="91"/>
<wire x1="38.1" y1="55.88" x2="40.64" y2="55.88" width="0.1524" layer="91"/>
<wire x1="40.64" y1="55.88" x2="40.64" y2="53.34" width="0.1524" layer="91"/>
<wire x1="40.64" y1="53.34" x2="53.34" y2="53.34" width="0.1524" layer="91"/>
<junction x="53.34" y="53.34"/>
<pinref part="OK1" gate="G$1" pin="C"/>
<pinref part="J4" gate="G$1" pin="5"/>
<pinref part="D3" gate="G$1" pin="A"/>
</segment>
</net>
<net name="N$23" class="0">
<segment>
<wire x1="55.88" y1="63.5" x2="53.34" y2="63.5" width="0.1524" layer="91"/>
<wire x1="53.34" y1="63.5" x2="53.34" y2="60.96" width="0.1524" layer="91"/>
<wire x1="53.34" y1="63.5" x2="50.8" y2="63.5" width="0.1524" layer="91"/>
<junction x="53.34" y="63.5"/>
<pinref part="OK1" gate="G$1" pin="A"/>
<pinref part="R9" gate="G$1" pin="1"/>
<pinref part="D3" gate="G$1" pin="C"/>
</segment>
</net>
<net name="N$25" class="0">
<segment>
<wire x1="38.1" y1="60.96" x2="40.64" y2="60.96" width="0.1524" layer="91"/>
<wire x1="40.64" y1="60.96" x2="40.64" y2="63.5" width="0.1524" layer="91"/>
<pinref part="J4" gate="G$1" pin="4"/>
<pinref part="R9" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$26" class="0">
<segment>
<wire x1="38.1" y1="93.98" x2="40.64" y2="93.98" width="0.1524" layer="91"/>
<pinref part="J2" gate="G$1" pin="4"/>
<pinref part="R13" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$27" class="0">
<segment>
<wire x1="45.72" y1="88.9" x2="38.1" y2="88.9" width="0.1524" layer="91"/>
<pinref part="R12" gate="G$1" pin="2"/>
<pinref part="J2" gate="G$1" pin="5"/>
</segment>
</net>
<net name="BL+" class="0">
<segment>
<wire x1="226.06" y1="58.42" x2="238.76" y2="58.42" width="0.1524" layer="91"/>
<wire x1="238.76" y1="58.42" x2="238.76" y2="60.96" width="0.1524" layer="91"/>
<wire x1="238.76" y1="60.96" x2="246.38" y2="60.96" width="0.1524" layer="91"/>
<label x="241.3" y="60.96" size="1.4224" layer="95"/>
<pinref part="J5" gate="G$1" pin="15"/>
<pinref part="R14" gate="G$1" pin="1"/>
</segment>
</net>
<net name="BL-" class="0">
<segment>
<wire x1="226.06" y1="55.88" x2="241.3" y2="55.88" width="0.1524" layer="91"/>
<wire x1="241.3" y1="55.88" x2="241.3" y2="58.42" width="0.1524" layer="91"/>
<wire x1="241.3" y1="58.42" x2="246.38" y2="58.42" width="0.1524" layer="91"/>
<label x="241.3" y="58.42" size="1.4224" layer="95"/>
<pinref part="Q1" gate="G$1" pin="D"/>
<pinref part="J5" gate="G$1" pin="16"/>
</segment>
</net>
<net name="N$22" class="0">
<segment>
<wire x1="208.28" y1="48.26" x2="220.98" y2="48.26" width="0.1524" layer="91"/>
<wire x1="208.28" y1="48.26" x2="208.28" y2="91.44" width="0.1524" layer="91"/>
<wire x1="175.26" y1="129.54" x2="190.5" y2="129.54" width="0.1524" layer="91"/>
<wire x1="190.5" y1="129.54" x2="190.5" y2="91.44" width="0.1524" layer="91"/>
<wire x1="208.28" y1="91.44" x2="190.5" y2="91.44" width="0.1524" layer="91"/>
<pinref part="Q1" gate="G$1" pin="G"/>
<pinref part="U1" gate="G$1" pin="PB3(AIN1/OC0A)"/>
</segment>
</net>
<net name="M_IN1" class="0">
<segment>
<wire x1="175.26" y1="76.2" x2="182.88" y2="76.2" width="0.1524" layer="91"/>
<wire x1="182.88" y1="58.42" x2="182.88" y2="76.2" width="0.1524" layer="91"/>
<wire x1="81.28" y1="58.42" x2="91.44" y2="58.42" width="0.1524" layer="91"/>
<wire x1="91.44" y1="58.42" x2="101.6" y2="58.42" width="0.1524" layer="91"/>
<wire x1="101.6" y1="58.42" x2="101.6" y2="60.96" width="0.1524" layer="91"/>
<wire x1="101.6" y1="58.42" x2="182.88" y2="58.42" width="0.1524" layer="91"/>
<junction x="91.44" y="58.42"/>
<junction x="101.6" y="58.42"/>
<label x="177.8" y="76.2" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PD0(RXD0)"/>
<pinref part="R11" gate="G$1" pin="1"/>
<pinref part="OK1" gate="G$1" pin="VO"/>
<pinref part="TP3" gate="G$1" pin="TP"/>
</segment>
</net>
<net name="M_OUT" class="0">
<segment>
<wire x1="106.68" y1="76.2" x2="106.68" y2="63.5" width="0.1524" layer="91"/>
<wire x1="45.72" y1="78.74" x2="45.72" y2="76.2" width="0.1524" layer="91"/>
<wire x1="45.72" y1="76.2" x2="106.68" y2="76.2" width="0.1524" layer="91"/>
<wire x1="106.68" y1="63.5" x2="185.42" y2="63.5" width="0.1524" layer="91"/>
<wire x1="185.42" y1="63.5" x2="185.42" y2="78.74" width="0.1524" layer="91"/>
<wire x1="175.26" y1="78.74" x2="185.42" y2="78.74" width="0.1524" layer="91"/>
<label x="177.8" y="78.74" size="1.4224" layer="95"/>
<pinref part="R12" gate="G$1" pin="1"/>
<pinref part="U1" gate="G$1" pin="PD1(TXD0)"/>
</segment>
</net>
<net name="M_IN2" class="0">
<segment>
<wire x1="106.68" y1="25.4" x2="106.68" y2="55.88" width="0.1524" layer="91"/>
<wire x1="187.96" y1="55.88" x2="106.68" y2="55.88" width="0.1524" layer="91"/>
<wire x1="187.96" y1="81.28" x2="187.96" y2="55.88" width="0.1524" layer="91"/>
<wire x1="175.26" y1="81.28" x2="187.96" y2="81.28" width="0.1524" layer="91"/>
<wire x1="81.28" y1="25.4" x2="91.44" y2="25.4" width="0.1524" layer="91"/>
<wire x1="91.44" y1="25.4" x2="101.6" y2="25.4" width="0.1524" layer="91"/>
<wire x1="101.6" y1="25.4" x2="101.6" y2="27.94" width="0.1524" layer="91"/>
<wire x1="106.68" y1="25.4" x2="101.6" y2="25.4" width="0.1524" layer="91"/>
<junction x="91.44" y="25.4"/>
<junction x="101.6" y="25.4"/>
<label x="177.8" y="81.28" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PD2(INT0/RXD1)"/>
<pinref part="R7" gate="G$1" pin="1"/>
<pinref part="OK2" gate="G$1" pin="VO"/>
<pinref part="TP4" gate="G$1" pin="TP"/>
</segment>
</net>
<net name="P$1" class="0">
<segment>
<wire x1="246.38" y1="116.84" x2="246.38" y2="119.38" width="0.1524" layer="91"/>
<wire x1="246.38" y1="119.38" x2="248.92" y2="119.38" width="0.1524" layer="91"/>
<wire x1="248.92" y1="121.92" x2="246.38" y2="121.92" width="0.1524" layer="91"/>
<wire x1="246.38" y1="121.92" x2="246.38" y2="119.38" width="0.1524" layer="91"/>
<junction x="246.38" y="119.38"/>
<pinref part="CGND1" gate="G$1" pin="P$1"/>
<pinref part="J1" gate="G$1" pin="S2"/>
<pinref part="J1" gate="G$1" pin="S1"/>
</segment>
<segment>
<wire x1="38.1" y1="73.66" x2="38.1" y2="76.2" width="0.1524" layer="91"/>
<wire x1="38.1" y1="76.2" x2="38.1" y2="78.74" width="0.1524" layer="91"/>
<wire x1="38.1" y1="78.74" x2="38.1" y2="81.28" width="0.1524" layer="91"/>
<junction x="38.1" y="73.66"/>
<junction x="38.1" y="76.2"/>
<junction x="38.1" y="78.74"/>
<pinref part="J2" gate="G$1" pin="PE@4"/>
<pinref part="CGND2" gate="G$1" pin="P$1"/>
<pinref part="J2" gate="G$1" pin="PE@3"/>
<pinref part="J2" gate="G$1" pin="PE@2"/>
<pinref part="J2" gate="G$1" pin="PE@1"/>
</segment>
<segment>
<wire x1="38.1" y1="40.64" x2="38.1" y2="43.18" width="0.1524" layer="91"/>
<wire x1="38.1" y1="43.18" x2="38.1" y2="45.72" width="0.1524" layer="91"/>
<wire x1="38.1" y1="45.72" x2="38.1" y2="48.26" width="0.1524" layer="91"/>
<junction x="38.1" y="40.64"/>
<junction x="38.1" y="43.18"/>
<junction x="38.1" y="45.72"/>
<pinref part="J4" gate="G$1" pin="PE@4"/>
<pinref part="CGND3" gate="G$1" pin="P$1"/>
<pinref part="J4" gate="G$1" pin="PE@3"/>
<pinref part="J4" gate="G$1" pin="PE@2"/>
<pinref part="J4" gate="G$1" pin="PE@1"/>
</segment>
<segment>
<wire x1="38.1" y1="7.62" x2="38.1" y2="10.16" width="0.1524" layer="91"/>
<wire x1="38.1" y1="10.16" x2="38.1" y2="12.7" width="0.1524" layer="91"/>
<wire x1="38.1" y1="12.7" x2="38.1" y2="15.24" width="0.1524" layer="91"/>
<junction x="38.1" y="7.62"/>
<junction x="38.1" y="10.16"/>
<junction x="38.1" y="12.7"/>
<pinref part="J3" gate="G$1" pin="PE@4"/>
<pinref part="CGND4" gate="G$1" pin="P$1"/>
<pinref part="J3" gate="G$1" pin="PE@3"/>
<pinref part="J3" gate="G$1" pin="PE@2"/>
<pinref part="J3" gate="G$1" pin="PE@1"/>
</segment>
<segment>
<wire x1="99.06" y1="165.1" x2="101.6" y2="165.1" width="0.1524" layer="91"/>
<wire x1="101.6" y1="165.1" x2="101.6" y2="162.56" width="0.1524" layer="91"/>
<wire x1="124.46" y1="167.64" x2="116.84" y2="167.64" width="0.1524" layer="91"/>
<wire x1="116.84" y1="167.64" x2="109.22" y2="167.64" width="0.1524" layer="91"/>
<wire x1="109.22" y1="167.64" x2="101.6" y2="167.64" width="0.1524" layer="91"/>
<wire x1="101.6" y1="167.64" x2="101.6" y2="165.1" width="0.1524" layer="91"/>
<junction x="116.84" y="167.64"/>
<junction x="109.22" y="167.64"/>
<junction x="101.6" y="167.64"/>
<junction x="101.6" y="165.1"/>
<pinref part="R21" gate="G$1" pin="1"/>
<pinref part="CGND5" gate="G$1" pin="P$1"/>
<pinref part="HOLE4" gate="G$1" pin="TP"/>
<pinref part="HOLE3" gate="G$1" pin="TP"/>
<pinref part="HOLE2" gate="G$1" pin="TP"/>
<pinref part="HOLE1" gate="G$1" pin="TP"/>
</segment>
</net>
<net name="N$34" class="0">
<segment>
<wire x1="60.96" y1="152.4" x2="60.96" y2="154.94" width="0.1524" layer="91"/>
<wire x1="60.96" y1="154.94" x2="71.12" y2="154.94" width="0.1524" layer="91"/>
<wire x1="71.12" y1="154.94" x2="76.2" y2="154.94" width="0.1524" layer="91"/>
<wire x1="71.12" y1="154.94" x2="71.12" y2="157.48" width="0.1524" layer="91"/>
<wire x1="48.26" y1="154.94" x2="50.8" y2="154.94" width="0.1524" layer="91"/>
<wire x1="60.96" y1="154.94" x2="50.8" y2="154.94" width="0.1524" layer="91"/>
<wire x1="76.2" y1="154.94" x2="76.2" y2="81.28" width="0.1524" layer="91"/>
<wire x1="76.2" y1="81.28" x2="114.3" y2="81.28" width="0.1524" layer="91"/>
<wire x1="114.3" y1="81.28" x2="114.3" y2="78.74" width="0.1524" layer="91"/>
<wire x1="114.3" y1="78.74" x2="129.54" y2="78.74" width="0.1524" layer="91"/>
<junction x="60.96" y="154.94"/>
<junction x="71.12" y="154.94"/>
<junction x="50.8" y="154.94"/>
<pinref part="R16" gate="G$1" pin="1"/>
<pinref part="C8" gate="G$1" pin="1"/>
<pinref part="TP5" gate="G$1" pin="TP"/>
<pinref part="R17" gate="G$1" pin="2"/>
<pinref part="R15" gate="G$1" pin="1"/>
<pinref part="U1" gate="G$1" pin="PA1(ADC1)"/>
</segment>
</net>
<net name="N$33" class="0">
<segment>
<wire x1="58.42" y1="121.92" x2="58.42" y2="124.46" width="0.1524" layer="91"/>
<wire x1="58.42" y1="124.46" x2="68.58" y2="124.46" width="0.1524" layer="91"/>
<wire x1="68.58" y1="124.46" x2="73.66" y2="124.46" width="0.1524" layer="91"/>
<wire x1="68.58" y1="124.46" x2="68.58" y2="127" width="0.1524" layer="91"/>
<wire x1="45.72" y1="124.46" x2="48.26" y2="124.46" width="0.1524" layer="91"/>
<wire x1="58.42" y1="124.46" x2="48.26" y2="124.46" width="0.1524" layer="91"/>
<wire x1="129.54" y1="76.2" x2="111.76" y2="76.2" width="0.1524" layer="91"/>
<wire x1="111.76" y1="76.2" x2="111.76" y2="78.74" width="0.1524" layer="91"/>
<wire x1="111.76" y1="78.74" x2="73.66" y2="78.74" width="0.1524" layer="91"/>
<wire x1="73.66" y1="78.74" x2="73.66" y2="124.46" width="0.1524" layer="91"/>
<junction x="58.42" y="124.46"/>
<junction x="68.58" y="124.46"/>
<junction x="48.26" y="124.46"/>
<pinref part="R19" gate="G$1" pin="1"/>
<pinref part="C9" gate="G$1" pin="1"/>
<pinref part="TP6" gate="G$1" pin="TP"/>
<pinref part="R20" gate="G$1" pin="2"/>
<pinref part="R18" gate="G$1" pin="1"/>
<pinref part="U1" gate="G$1" pin="PA0(ADC0)"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<wire x1="213.36" y1="167.64" x2="210.82" y2="167.64" width="0.1524" layer="91"/>
<pinref part="R23" gate="G$1" pin="1"/>
<pinref part="D8" gate="G$1" pin="C"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<wire x1="213.36" y1="157.48" x2="210.82" y2="157.48" width="0.1524" layer="91"/>
<pinref part="D9" gate="G$1" pin="C"/>
<pinref part="R22" gate="G$1" pin="1"/>
</segment>
</net>
<net name="N$21" class="0">
<segment>
<wire x1="175.26" y1="132.08" x2="190.5" y2="132.08" width="0.1524" layer="91"/>
<wire x1="190.5" y1="132.08" x2="190.5" y2="157.48" width="0.1524" layer="91"/>
<wire x1="190.5" y1="157.48" x2="200.66" y2="157.48" width="0.1524" layer="91"/>
<pinref part="U1" gate="G$1" pin="PB4(SS/OC0B)"/>
<pinref part="R22" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$24" class="0">
<segment>
<wire x1="25.4" y1="154.94" x2="33.02" y2="154.94" width="0.1524" layer="91"/>
<wire x1="33.02" y1="154.94" x2="38.1" y2="154.94" width="0.1524" layer="91"/>
<wire x1="33.02" y1="154.94" x2="33.02" y2="152.4" width="0.1524" layer="91"/>
<junction x="33.02" y="154.94"/>
<pinref part="J8" gate="G$1" pin="2"/>
<pinref part="R17" gate="G$1" pin="1"/>
<pinref part="D6" gate="G$1" pin="C"/>
</segment>
</net>
<net name="N$28" class="0">
<segment>
<wire x1="25.4" y1="124.46" x2="33.02" y2="124.46" width="0.1524" layer="91"/>
<wire x1="33.02" y1="124.46" x2="35.56" y2="124.46" width="0.1524" layer="91"/>
<wire x1="33.02" y1="121.92" x2="33.02" y2="124.46" width="0.1524" layer="91"/>
<junction x="33.02" y="124.46"/>
<pinref part="J7" gate="G$1" pin="2"/>
<pinref part="R20" gate="G$1" pin="1"/>
<pinref part="D5" gate="G$1" pin="C"/>
</segment>
</net>
<net name="N$29" class="0">
<segment>
<wire x1="129.54" y1="104.14" x2="119.38" y2="104.14" width="0.1524" layer="91"/>
<wire x1="119.38" y1="104.14" x2="119.38" y2="106.68" width="0.1524" layer="91"/>
<wire x1="119.38" y1="106.68" x2="114.3" y2="106.68" width="0.1524" layer="91"/>
<pinref part="U1" gate="G$1" pin="AREF"/>
<pinref part="C6" gate="G$1" pin="1"/>
</segment>
</net>
<net name="GC2" class="0">
<segment>
<wire x1="129.54" y1="35.56" x2="121.92" y2="35.56" width="0.1524" layer="91"/>
<wire x1="121.92" y1="35.56" x2="119.38" y2="33.02" width="0.1524" layer="91"/>
<label x="124.46" y="35.56" size="1.4224" layer="95"/>
<pinref part="J6" gate="G$1" pin="3"/>
</segment>
<segment>
<wire x1="129.54" y1="93.98" x2="121.92" y2="93.98" width="0.1524" layer="91"/>
<wire x1="121.92" y1="93.98" x2="119.38" y2="91.44" width="0.1524" layer="91"/>
<label x="124.46" y="93.98" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PA7(ADC7)"/>
</segment>
</net>
<net name="GC1" class="0">
<segment>
<wire x1="144.78" y1="38.1" x2="149.86" y2="38.1" width="0.1524" layer="91"/>
<wire x1="149.86" y1="38.1" x2="149.86" y2="48.26" width="0.1524" layer="91"/>
<wire x1="149.86" y1="48.26" x2="193.04" y2="48.26" width="0.1524" layer="91"/>
<wire x1="193.04" y1="48.26" x2="193.04" y2="83.82" width="0.1524" layer="91"/>
<wire x1="193.04" y1="83.82" x2="175.26" y2="83.82" width="0.1524" layer="91"/>
<wire x1="149.86" y1="38.1" x2="152.4" y2="38.1" width="0.1524" layer="91"/>
<wire x1="152.4" y1="38.1" x2="154.94" y2="35.56" width="0.1524" layer="91"/>
<junction x="149.86" y="38.1"/>
<label x="144.78" y="38.1" size="1.4224" layer="95"/>
<pinref part="J6" gate="G$1" pin="2"/>
<pinref part="U1" gate="G$1" pin="PD3(INT1/TXD1)"/>
</segment>
<segment>
<wire x1="129.54" y1="91.44" x2="121.92" y2="91.44" width="0.1524" layer="91"/>
<wire x1="121.92" y1="91.44" x2="119.38" y2="88.9" width="0.1524" layer="91"/>
<label x="124.46" y="91.44" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PA6(ADC6)"/>
</segment>
</net>
<net name="PUSH" class="0">
<segment>
<wire x1="144.78" y1="35.56" x2="152.4" y2="35.56" width="0.1524" layer="91"/>
<wire x1="152.4" y1="35.56" x2="154.94" y2="33.02" width="0.1524" layer="91"/>
<label x="144.78" y="35.56" size="1.4224" layer="95"/>
<pinref part="J6" gate="G$1" pin="4"/>
</segment>
<segment>
<wire x1="129.54" y1="88.9" x2="121.92" y2="88.9" width="0.1524" layer="91"/>
<wire x1="121.92" y1="88.9" x2="119.38" y2="86.36" width="0.1524" layer="91"/>
<label x="124.46" y="88.9" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PA5(ADC5)"/>
</segment>
</net>
<net name="SEL" class="0">
<segment>
<wire x1="129.54" y1="33.02" x2="121.92" y2="33.02" width="0.1524" layer="91"/>
<wire x1="121.92" y1="33.02" x2="119.38" y2="30.48" width="0.1524" layer="91"/>
<label x="124.46" y="33.02" size="1.4224" layer="95"/>
<pinref part="J6" gate="G$1" pin="5"/>
</segment>
<segment>
<wire x1="129.54" y1="86.36" x2="121.92" y2="86.36" width="0.1524" layer="91"/>
<wire x1="121.92" y1="86.36" x2="119.38" y2="83.82" width="0.1524" layer="91"/>
<label x="124.46" y="86.36" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PA4(ADC4)"/>
</segment>
</net>
<net name="BACK" class="0">
<segment>
<wire x1="144.78" y1="33.02" x2="152.4" y2="33.02" width="0.1524" layer="91"/>
<wire x1="152.4" y1="33.02" x2="154.94" y2="30.48" width="0.1524" layer="91"/>
<label x="144.78" y="33.02" size="1.4224" layer="95"/>
<pinref part="J6" gate="G$1" pin="6"/>
</segment>
<segment>
<wire x1="129.54" y1="83.82" x2="121.92" y2="83.82" width="0.1524" layer="91"/>
<wire x1="121.92" y1="83.82" x2="119.38" y2="81.28" width="0.1524" layer="91"/>
<label x="124.46" y="83.82" size="1.4224" layer="95"/>
<pinref part="U1" gate="G$1" pin="PA3(ADC3)"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
