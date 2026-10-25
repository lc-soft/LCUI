import { ScrollArea, ScrollAreaContent, Scrollbar, Text, Widget } from "@lcui/react";
import DemoProvider from "../../components/demo-provider";
import CodeBlockCopy from "../../components/code-block-copy";

export default function FieldBasicDemo() {
  return (
    <DemoProvider>
      <Widget $ref="preview" className="demo-preview" />
      <Widget className="code-block">
        <Widget className="code-block-header">
          <Widget className="demo-files">
            <Text className="demo-tab active" data-language="tsx" data-value="example.tsx">example.tsx</Text>
            <Text className="demo-tab" data-language="tsx" data-value="main.c" style={{ display: "none" }}>main.c</Text>
            <Text className="demo-tab" data-language="c" data-value="main.c" style={{ display: "none" }}>main.c</Text>
            <Text className="demo-tab" data-language="xml" data-value="ui.xml" style={{ display: "none" }}>ui.xml</Text>
            <Text className="demo-tab" data-language="xml" data-value="main.c" style={{ display: "none" }}>main.c</Text>
          </Widget>
          <Widget className="demo-languages">
            <Text className="demo-tab active" data-value="tsx">TSX</Text>
            <Text className="demo-tab" data-value="c">C</Text>
            <Text className="demo-tab" data-value="xml">XML</Text>
          </Widget>
        </Widget>
        <Widget className="code-block-body active" data-language="tsx" data-file="example.tsx" data-source="examples/field-basic/5">
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#cf222e]import[/color] {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`  [color=#8250df]Button[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`  [color=#8250df]Checkbox[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`  [color=#8250df]Field[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`  [color=#8250df]FieldDescription[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`  [color=#8250df]FieldGroup[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`  [color=#8250df]FieldLabel[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`  [color=#8250df]FieldLegend[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`  [color=#8250df]FieldSeparator[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`  [color=#8250df]FieldSet[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`  [color=#8250df]Select[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`  [color=#8250df]SelectContent[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`  [color=#8250df]SelectGroup[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`  [color=#8250df]SelectItem[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`  [color=#8250df]SelectTrigger[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`  [color=#8250df]SelectValue[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`  [color=#8250df]TextInput[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`  [color=#8250df]Widget[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`} [color=#cf222e]from[/color] [color=#0a3069]"@lcui/react"[/color];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`[color=#cf222e]const[/color] months = \\[`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`  [color=#0a3069]"01"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`  [color=#0a3069]"02"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`  [color=#0a3069]"03"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`  [color=#0a3069]"04"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`  [color=#0a3069]"05"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`  [color=#0a3069]"06"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`  [color=#0a3069]"07"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`  [color=#0a3069]"08"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`  [color=#0a3069]"09"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`  [color=#0a3069]"10"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`  [color=#0a3069]"11"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`  [color=#0a3069]"12"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`[color=#cf222e]const[/color] years = \\[[color=#0a3069]"2024"[/color], [color=#0a3069]"2025"[/color], [color=#0a3069]"2026"[/color], [color=#0a3069]"2027"[/color], [color=#0a3069]"2028"[/color], [color=#0a3069]"2029"[/color]];`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`[color=#cf222e]export[/color] [color=#cf222e]default[/color] [color=#cf222e]function[/color] [color=#8250df]App[/color]([color=#0550ae][/color]) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`  [color=#cf222e]return[/color] (`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]Widget[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex flex-col gap-5 max-w-\\[448px] p-1 -m-1"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldLegend[/color]>[/color]Payment Method[color=#116329]</[color=#0550ae]FieldLegend[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`          All transactions are secure and encrypted`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"name-on-card"[/color]>[/color]Name on Card[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"name-on-card"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Evil Rabbit"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-number"[/color]>[/color]Card Number[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-number"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"1234 5678 9012 3456"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]Enter your 16-digit card number[color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex gap-4"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex-1"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-month"[/color]>[/color]Month[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Select[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SelectTrigger[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-month"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SelectValue[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"MM"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">64</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">65</Text>
                <Text className="code-line">{`                    {months.map((item) => (`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">66</Text>
                <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]key[/color]=[color=#0a3069]{item}[/color] [color=#0550ae]value[/color]=[color=#0a3069]{item}[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">67</Text>
                <Text className="code-line">{`                        {item}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">68</Text>
                <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">69</Text>
                <Text className="code-line">{`                    ))}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">70</Text>
                <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">71</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">72</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Select[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">73</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">74</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex-1"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">75</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-year"[/color]>[/color]Year[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">76</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]Select[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">77</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SelectTrigger[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-year"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">78</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SelectValue[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"YYYY"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">79</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]SelectTrigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">80</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">81</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">82</Text>
                <Text className="code-line">{`                    {years.map((item) => (`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">83</Text>
                <Text className="code-line">{`                      [color=#116329]<[color=#0550ae]SelectItem[/color] [color=#0550ae]key[/color]=[color=#0a3069]{item}[/color] [color=#0550ae]value[/color]=[color=#0a3069]{item}[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">84</Text>
                <Text className="code-line">{`                        {item}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">85</Text>
                <Text className="code-line">{`                      [color=#116329]</[color=#0550ae]SelectItem[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">86</Text>
                <Text className="code-line">{`                    ))}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">87</Text>
                <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]SelectGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">88</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]SelectContent[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">89</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]Select[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">90</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">91</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]className[/color]=[color=#0a3069]"flex-1"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">92</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"card-cvv"[/color]>[/color]CVV[color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">93</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]TextInput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-cvv"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"123"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">94</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">95</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">96</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">97</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">98</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSeparator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">99</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">100</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldLegend[/color]>[/color]Billing Address[color=#116329]</[color=#0550ae]FieldLegend[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">101</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">102</Text>
                <Text className="code-line">{`          The billing address associated with your payment method`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">103</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldDescription[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">104</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">105</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">106</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]Checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"billing-same"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">107</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]FieldLabel[/color] [color=#0550ae]htmlFor[/color]=[color=#0a3069]"billing-same"[/color] [color=#0550ae]className[/color]=[color=#0a3069]"font-normal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">108</Text>
                <Text className="code-line">{`              Same as shipping address`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">109</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]FieldLabel[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">110</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">111</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]FieldGroup[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">112</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]FieldSet[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">113</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]Field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">114</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]Submit[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">115</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]Button[/color]>[/color]Cancel[color=#116329]</[color=#0550ae]Button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">116</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]Field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">117</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]Widget[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">118</Text>
                <Text className="code-line">{`  );`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">119</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="tsx" data-file="main.c" data-source="examples/field-basic/6" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#6e7781] * docs/examples/field-basic-tsx/main.c: -- Field widget demo (TSX variant)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]"example.h"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_basic_tsx_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`        example_load(parent);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="c" data-file="main.c" data-source="examples/field-basic/7" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#6e7781] * docs/examples/field-basic/main.c: -- Field widget demo (C variant)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *month_items\\[[color=#0550ae]12[/color]] = { [color=#0a3069]"01"[/color], [color=#0a3069]"02"[/color], [color=#0a3069]"03"[/color], [color=#0a3069]"04"[/color], [color=#0a3069]"05"[/color], [color=#0a3069]"06"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`                                       [color=#0a3069]"07"[/color], [color=#0a3069]"08"[/color], [color=#0a3069]"09"[/color], [color=#0a3069]"10"[/color], [color=#0a3069]"11"[/color], [color=#0a3069]"12"[/color] };`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]const[/color] [color=#953800]char[/color] *year_items\\[[color=#0550ae]6[/color]] = { [color=#0a3069]"2024"[/color], [color=#0a3069]"2025"[/color], [color=#0a3069]"2026"[/color],`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`                                     [color=#0a3069]"2027"[/color], [color=#0a3069]"2028"[/color], [color=#0a3069]"2029"[/color] };`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`[color=#6e7781]/* Build a select and point the field label at its real trigger button. */[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_t[/color] *[color=#8250df]create_select[/color][color=#0550ae]([color=#953800]const[/color] [color=#953800]char[/color] *id, [color=#953800]const[/color] [color=#953800]char[/color] *placeholder,[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`[color=#0550ae]                                  [color=#953800]const[/color] [color=#953800]char[/color] *[color=#953800]const[/color] *items, [color=#953800]int[/color] count)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *select = ui_create_select();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *button;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`        [color=#953800]int[/color] i;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`        ui_select_set_placeholder(select, placeholder);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`        [color=#cf222e]for[/color] (i = [color=#0550ae]0[/color]; i < count; ++i) {`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`                ui_select_add_item(select, items\\[i], items\\[i]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`        }`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`        button = ui_widget_get_child(ui_widget_get_child(select, [color=#0550ae]0[/color]), [color=#0550ae]0[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`        ui_widget_set_id(button, id);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] select;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]field_basic_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *parent)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *outer;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *set1, *legend1, *desc1, *group1;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *name_field, *name_lbl, *name_input;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card_field, *card_lbl, *card_input, *card_desc;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *card_row;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *month_field, *month_lbl, *month_select;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *year_field, *year_lbl, *year_select;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *cvv_field, *cvv_lbl, *cvv_input;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *sep;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *set2, *legend2, *desc2, *group2;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *same_field, *same_cb, *same_lbl;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *actions, *submit, *cancel;`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`        outer = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`        set1 = ui_create_field_set();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`        legend1 = ui_create_field_legend();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`        desc1 = ui_create_field_description();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`        group1 = ui_create_field_group();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`        name_field = ui_create_field();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`        name_lbl = ui_create_field_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`        name_input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`        card_field = ui_create_field();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`        card_lbl = ui_create_field_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`        card_input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{`        card_desc = ui_create_field_description();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{`        card_row = ui_create_widget([color=#0550ae]NULL[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`        month_field = ui_create_field();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`        month_lbl = ui_create_field_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`        year_field = ui_create_field();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`        year_lbl = ui_create_field_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{`        cvv_field = ui_create_field();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">64</Text>
                <Text className="code-line">{`        cvv_lbl = ui_create_field_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">65</Text>
                <Text className="code-line">{`        cvv_input = ui_create_widget([color=#0a3069]"textinput"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">66</Text>
                <Text className="code-line">{`        sep = ui_create_field_separator();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">67</Text>
                <Text className="code-line">{`        set2 = ui_create_field_set();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">68</Text>
                <Text className="code-line">{`        legend2 = ui_create_field_legend();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">69</Text>
                <Text className="code-line">{`        desc2 = ui_create_field_description();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">70</Text>
                <Text className="code-line">{`        group2 = ui_create_field_group();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">71</Text>
                <Text className="code-line">{`        same_field = ui_create_field();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">72</Text>
                <Text className="code-line">{`        same_cb = ui_create_checkbox();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">73</Text>
                <Text className="code-line">{`        same_lbl = ui_create_field_label();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">74</Text>
                <Text className="code-line">{`        ui_widget_add_class(same_lbl, [color=#0a3069]"font-normal"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">75</Text>
                <Text className="code-line">{`        actions = ui_create_field();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">76</Text>
                <Text className="code-line">{`        submit = ui_create_widget([color=#0a3069]"button"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">77</Text>
                <Text className="code-line">{`        cancel = ui_create_widget([color=#0a3069]"button"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">78</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">79</Text>
                <Text className="code-line">{`        ui_widget_add_class(outer, [color=#0a3069]"field-demo"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">80</Text>
                <Text className="code-line">{`        ui_widget_add_class(outer, [color=#0a3069]"field-basic-demo"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">81</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">82</Text>
                <Text className="code-line">{`        ui_text_set_content(legend1, [color=#0a3069]"Payment Method"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">83</Text>
                <Text className="code-line">{`        ui_widget_append(set1, legend1);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">84</Text>
                <Text className="code-line">{`        ui_text_set_content(desc1, [color=#0a3069]"All transactions are secure and encrypted"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">85</Text>
                <Text className="code-line">{`        ui_widget_append(set1, desc1);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">86</Text>
                <Text className="code-line">{`        ui_widget_append(set1, group1);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">87</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">88</Text>
                <Text className="code-line">{`        ui_label_set_for(name_lbl, [color=#0a3069]"name-on-card"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">89</Text>
                <Text className="code-line">{`        ui_text_set_content(name_lbl, [color=#0a3069]"Name on Card"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">90</Text>
                <Text className="code-line">{`        ui_widget_append(name_field, name_lbl);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">91</Text>
                <Text className="code-line">{`        ui_textinput_set_placeholder(name_input, [color=#0a3069]"Evil Rabbit"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">92</Text>
                <Text className="code-line">{`        ui_widget_set_id(name_input, [color=#0a3069]"name-on-card"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">93</Text>
                <Text className="code-line">{`        ui_widget_append(name_field, name_input);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">94</Text>
                <Text className="code-line">{`        ui_widget_append(group1, name_field);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">95</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">96</Text>
                <Text className="code-line">{`        ui_label_set_for(card_lbl, [color=#0a3069]"card-number"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">97</Text>
                <Text className="code-line">{`        ui_text_set_content(card_lbl, [color=#0a3069]"Card Number"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">98</Text>
                <Text className="code-line">{`        ui_widget_append(card_field, card_lbl);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">99</Text>
                <Text className="code-line">{`        ui_textinput_set_placeholder(card_input, [color=#0a3069]"1234 5678 9012 3456"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">100</Text>
                <Text className="code-line">{`        ui_widget_set_id(card_input, [color=#0a3069]"card-number"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">101</Text>
                <Text className="code-line">{`        ui_widget_append(card_field, card_input);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">102</Text>
                <Text className="code-line">{`        ui_text_set_content(card_desc, [color=#0a3069]"Enter your 16-digit card number"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">103</Text>
                <Text className="code-line">{`        ui_widget_append(card_field, card_desc);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">104</Text>
                <Text className="code-line">{`        ui_widget_append(group1, card_field);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">105</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">106</Text>
                <Text className="code-line">{`        ui_widget_add_class(card_row, [color=#0a3069]"field-basic-card-row"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">107</Text>
                <Text className="code-line">{`        ui_widget_add_class(month_field, [color=#0a3069]"field-basic-card-field"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">108</Text>
                <Text className="code-line">{`        ui_widget_add_class(year_field, [color=#0a3069]"field-basic-card-field"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">109</Text>
                <Text className="code-line">{`        ui_widget_add_class(cvv_field, [color=#0a3069]"field-basic-card-field"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">110</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">111</Text>
                <Text className="code-line">{`        ui_label_set_for(month_lbl, [color=#0a3069]"card-month"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">112</Text>
                <Text className="code-line">{`        ui_text_set_content(month_lbl, [color=#0a3069]"Month"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">113</Text>
                <Text className="code-line">{`        ui_widget_append(month_field, month_lbl);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">114</Text>
                <Text className="code-line">{`        month_select = create_select([color=#0a3069]"card-month"[/color], [color=#0a3069]"MM"[/color], month_items, [color=#0550ae]12[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">115</Text>
                <Text className="code-line">{`        ui_widget_append(month_field, month_select);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">116</Text>
                <Text className="code-line">{`        ui_widget_append(card_row, month_field);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">117</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">118</Text>
                <Text className="code-line">{`        ui_label_set_for(year_lbl, [color=#0a3069]"card-year"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">119</Text>
                <Text className="code-line">{`        ui_text_set_content(year_lbl, [color=#0a3069]"Year"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">120</Text>
                <Text className="code-line">{`        ui_widget_append(year_field, year_lbl);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">121</Text>
                <Text className="code-line">{`        year_select = create_select([color=#0a3069]"card-year"[/color], [color=#0a3069]"YYYY"[/color], year_items, [color=#0550ae]6[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">122</Text>
                <Text className="code-line">{`        ui_widget_append(year_field, year_select);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">123</Text>
                <Text className="code-line">{`        ui_widget_append(card_row, year_field);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">124</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">125</Text>
                <Text className="code-line">{`        ui_label_set_for(cvv_lbl, [color=#0a3069]"card-cvv"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">126</Text>
                <Text className="code-line">{`        ui_text_set_content(cvv_lbl, [color=#0a3069]"CVV"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">127</Text>
                <Text className="code-line">{`        ui_widget_append(cvv_field, cvv_lbl);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">128</Text>
                <Text className="code-line">{`        ui_textinput_set_placeholder(cvv_input, [color=#0a3069]"123"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">129</Text>
                <Text className="code-line">{`        ui_widget_set_id(cvv_input, [color=#0a3069]"card-cvv"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">130</Text>
                <Text className="code-line">{`        ui_widget_append(cvv_field, cvv_input);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">131</Text>
                <Text className="code-line">{`        ui_widget_append(card_row, cvv_field);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">132</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">133</Text>
                <Text className="code-line">{`        ui_widget_append(group1, card_row);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">134</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">135</Text>
                <Text className="code-line">{`        ui_widget_append(outer, set1);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">136</Text>
                <Text className="code-line">{`        ui_widget_append(outer, sep);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">137</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">138</Text>
                <Text className="code-line">{`        ui_text_set_content(legend2, [color=#0a3069]"Billing Address"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">139</Text>
                <Text className="code-line">{`        ui_widget_append(set2, legend2);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">140</Text>
                <Text className="code-line">{`        ui_text_set_content(`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">141</Text>
                <Text className="code-line">{`            desc2, [color=#0a3069]"The billing address associated with your payment method"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">142</Text>
                <Text className="code-line">{`        ui_widget_append(set2, desc2);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">143</Text>
                <Text className="code-line">{`        ui_widget_append(set2, group2);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">144</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">145</Text>
                <Text className="code-line">{`        ui_field_set_orientation(same_field, [color=#0a3069]"horizontal"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">146</Text>
                <Text className="code-line">{`        ui_widget_set_id(same_cb, [color=#0a3069]"billing-same"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">147</Text>
                <Text className="code-line">{`        ui_widget_append(same_field, same_cb);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">148</Text>
                <Text className="code-line">{`        ui_label_set_for(same_lbl, [color=#0a3069]"billing-same"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">149</Text>
                <Text className="code-line">{`        ui_text_set_content(same_lbl, [color=#0a3069]"Same as shipping address"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">150</Text>
                <Text className="code-line">{`        ui_widget_append(same_field, same_lbl);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">151</Text>
                <Text className="code-line">{`        ui_widget_append(group2, same_field);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">152</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">153</Text>
                <Text className="code-line">{`        ui_widget_append(outer, set2);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">154</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">155</Text>
                <Text className="code-line">{`        ui_field_set_orientation(actions, [color=#0a3069]"horizontal"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">156</Text>
                <Text className="code-line">{`        ui_button_set_text(submit, [color=#0a3069]"Submit"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">157</Text>
                <Text className="code-line">{`        ui_button_set_variant(submit, UI_BUTTON_VARIANT_PRIMARY);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">158</Text>
                <Text className="code-line">{`        ui_widget_append(actions, submit);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">159</Text>
                <Text className="code-line">{`        ui_button_set_text(cancel, [color=#0a3069]"Cancel"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">160</Text>
                <Text className="code-line">{`        ui_widget_append(actions, cancel);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">161</Text>
                <Text className="code-line">{`        ui_widget_append(outer, actions);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">162</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">163</Text>
                <Text className="code-line">{`        ui_widget_append(parent, outer);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">164</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="ui.xml" data-source="examples/field-basic/8" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]<?xml version=[color=#0a3069]"1.0"[/color] encoding=[color=#0a3069]"UTF-8"[/color] ?>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#116329]<[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`  [color=#116329]<[color=#0550ae]ui[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`    [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-demo field-basic-demo"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-legend[/color]>[/color]Payment Method[color=#116329]</[color=#0550ae]field-legend[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{`[color=#116329]        >[/color]All transactions are secure and encrypted[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"name-on-card"[/color]>[/color]Name on Card[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"name-on-card"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"Evil Rabbit"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">15</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-number"[/color]>[/color]Card Number[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">16</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-number"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"1234 5678 9012 3456"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">17</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">18</Text>
                <Text className="code-line">{`[color=#116329]            >[/color]Enter your 16-digit card number[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">19</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">20</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]div[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-basic-card-row"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">21</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-basic-card-field"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">22</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-month"[/color]>[/color]Month[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">23</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]select[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">24</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]select-trigger[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-month"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">25</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]select-value[/color] [color=#0550ae]data-select-placeholder[/color]=[color=#0a3069]"MM"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">26</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]select-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">27</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]portal[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">28</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]select-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">29</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"01"[/color]>[/color]01[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">30</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"02"[/color]>[/color]02[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">31</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"03"[/color]>[/color]03[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">32</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"04"[/color]>[/color]04[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">33</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"05"[/color]>[/color]05[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">34</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"06"[/color]>[/color]06[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">35</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"07"[/color]>[/color]07[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">36</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"08"[/color]>[/color]08[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">37</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"09"[/color]>[/color]09[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">38</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"10"[/color]>[/color]10[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">39</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"11"[/color]>[/color]11[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">40</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"12"[/color]>[/color]12[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">41</Text>
                <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]select-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">42</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]portal[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">43</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]select[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">44</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">45</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-basic-card-field"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">46</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-year"[/color]>[/color]Year[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">47</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]select[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">48</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]select-trigger[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-year"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">49</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]select-value[/color] [color=#0550ae]data-select-placeholder[/color]=[color=#0a3069]"YYYY"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">50</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]select-trigger[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">51</Text>
                <Text className="code-line">{`                [color=#116329]<[color=#0550ae]portal[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">52</Text>
                <Text className="code-line">{`                  [color=#116329]<[color=#0550ae]select-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">53</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2024"[/color]>[/color]2024[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">54</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2025"[/color]>[/color]2025[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">55</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2026"[/color]>[/color]2026[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">56</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2027"[/color]>[/color]2027[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">57</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2028"[/color]>[/color]2028[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">58</Text>
                <Text className="code-line">{`                    [color=#116329]<[color=#0550ae]select-item[/color] [color=#0550ae]value[/color]=[color=#0a3069]"2029"[/color]>[/color]2029[color=#116329]</[color=#0550ae]select-item[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">59</Text>
                <Text className="code-line">{`                  [color=#116329]</[color=#0550ae]select-content[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">60</Text>
                <Text className="code-line">{`                [color=#116329]</[color=#0550ae]portal[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">61</Text>
                <Text className="code-line">{`              [color=#116329]</[color=#0550ae]select[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">62</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">63</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]class[/color]=[color=#0a3069]"field-basic-card-field"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">64</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]field-label[/color] [color=#0550ae]for[/color]=[color=#0a3069]"card-cvv"[/color]>[/color]CVV[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">65</Text>
                <Text className="code-line">{`              [color=#116329]<[color=#0550ae]textinput[/color] [color=#0550ae]id[/color]=[color=#0a3069]"card-cvv"[/color] [color=#0550ae]placeholder[/color]=[color=#0a3069]"123"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">66</Text>
                <Text className="code-line">{`            [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">67</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">68</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">69</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">70</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-separator[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">71</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field-set[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">72</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-legend[/color]>[/color]Billing Address[color=#116329]</[color=#0550ae]field-legend[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">73</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-description[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">74</Text>
                <Text className="code-line">{`[color=#116329]        >[/color]The billing address associated with your payment method[color=#116329]</[color=#0550ae]field-description[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">75</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]field-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">76</Text>
                <Text className="code-line">{`          [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">77</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]checkbox[/color] [color=#0550ae]id[/color]=[color=#0a3069]"billing-same"[/color] />[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">78</Text>
                <Text className="code-line">{`            [color=#116329]<[color=#0550ae]field-label[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">79</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]class[/color]=[color=#0a3069]"font-normal"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">80</Text>
                <Text className="code-line">{`[color=#116329]              [color=#0550ae]for[/color]=[color=#0a3069]"billing-same"[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">81</Text>
                <Text className="code-line">{`[color=#116329]            >[/color]Same as shipping address[color=#116329]</[color=#0550ae]field-label[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">82</Text>
                <Text className="code-line">{`          [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">83</Text>
                <Text className="code-line">{`        [color=#116329]</[color=#0550ae]field-group[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">84</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field-set[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">85</Text>
                <Text className="code-line">{`      [color=#116329]<[color=#0550ae]field[/color] [color=#0550ae]orientation[/color]=[color=#0a3069]"horizontal"[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">86</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color] [color=#0550ae]variant[/color]=[color=#0a3069]"primary"[/color]>[/color]Submit[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">87</Text>
                <Text className="code-line">{`        [color=#116329]<[color=#0550ae]button[/color]>[/color]Cancel[color=#116329]</[color=#0550ae]button[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">88</Text>
                <Text className="code-line">{`      [color=#116329]</[color=#0550ae]field[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">89</Text>
                <Text className="code-line">{`    [color=#116329]</[color=#0550ae]div[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">90</Text>
                <Text className="code-line">{`  [color=#116329]</[color=#0550ae]ui[/color]>[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">91</Text>
                <Text className="code-line">{`[color=#116329]</[color=#0550ae]lcui-app[/color]>[/color]`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
        <Widget className="code-block-body" data-language="xml" data-file="main.c" data-source="examples/field-basic/9" style={{ display: "none" }}>
          <CodeBlockCopy />
          <ScrollArea className="code-block-scroll" style={{ height: "224px" }}>
            <ScrollAreaContent className="code-block-scroll-content">
              <Widget className="code-line-row">
                <Text className="code-line-number">1</Text>
                <Text className="code-line">{`[color=#6e7781]/*[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">2</Text>
                <Text className="code-line">{`[color=#6e7781] * docs/examples/field-basic-xml/main.c: -- Field widget demo (XML variant)[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">3</Text>
                <Text className="code-line">{`[color=#6e7781] *[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">4</Text>
                <Text className="code-line">{`[color=#6e7781] * SPDX-License-Identifier: MIT[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">5</Text>
                <Text className="code-line">{`[color=#6e7781] */[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">6</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">7</Text>
                <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">8</Text>
                <Text className="code-line">{` `}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">9</Text>
                <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">10</Text>
                <Text className="code-line">{`{`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">11</Text>
                <Text className="code-line">{`        lcui_init();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">12</Text>
                <Text className="code-line">{`        ui_load_xml_file([color=#0a3069]"ui.xml"[/color]);`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">13</Text>
                <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
              </Widget>
              <Widget className="code-line-row">
                <Text className="code-line-number">14</Text>
                <Text className="code-line">{`}`}</Text>
              </Widget>
            </ScrollAreaContent>
            <Scrollbar />
            <Scrollbar orientation="horizontal" />
          </ScrollArea>
        </Widget>
      </Widget>
    </DemoProvider>
  );
}
