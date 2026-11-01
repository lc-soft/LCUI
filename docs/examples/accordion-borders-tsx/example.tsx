import {
  Accordion,
  AccordionContent,
  AccordionItem,
  AccordionTrigger,
} from "@lcui/react";

export default function Example() {
  return (
    <Accordion
      className="w-[512px] rounded-lg border"
      defaultValue="billing"
    >
      <AccordionItem value="billing" className="px-4">
        <AccordionTrigger>How does billing work?</AccordionTrigger>
        <AccordionContent>
          We offer monthly and annual subscription plans. Billing is charged at
          the beginning of each cycle, and you can cancel anytime. All plans
          include automatic backups, 24/7 support, and unlimited team members.
        </AccordionContent>
      </AccordionItem>
      <AccordionItem value="security" className="px-4">
        <AccordionTrigger>Is my data secure?</AccordionTrigger>
        <AccordionContent>
          Yes. We use end-to-end encryption, SOC 2 Type II compliance, and
          regular third-party security audits. All data is encrypted at rest
          and in transit using industry-standard protocols.
        </AccordionContent>
      </AccordionItem>
      <AccordionItem value="integration" className="px-4">
        <AccordionTrigger>What integrations do you support?</AccordionTrigger>
        <AccordionContent>
          We integrate with 500+ popular tools including Slack, Zapier,
          Salesforce, HubSpot, and more. You can also build custom integrations
          using our REST API and webhooks.
        </AccordionContent>
      </AccordionItem>
    </Accordion>
  );
}
