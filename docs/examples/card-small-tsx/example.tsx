import {
  Button,
  Card,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
  Icon,
  Text,
  Widget,
} from "@lcui/react";

export default function App() {
  const features = [
    "Choose a schedule (daily, or weekly).",
    "Send to channels or specific teammates.",
    "Include charts, tables, and key metrics.",
  ];

  return (
    <Card size="sm" className="w-full max-w-xs">
      <CardHeader>
        <CardTitle>Scheduled reports</CardTitle>
        <CardDescription>
          Weekly snapshots. No more manual exports.
        </CardDescription>
      </CardHeader>
      <CardContent>
        <Widget className="flex flex-col gap-2 py-2">
          {features.map((feature) => (
            <Widget key={feature} className="flex gap-2">
              <Icon
                className="mt-0.5 shrink-0 text-muted-foreground"
                name="chevron-right"
                size={16}
              />
              <Text className="flex-1">{feature}</Text>
            </Widget>
          ))}
        </Widget>
      </CardContent>
      <CardFooter className="flex-col gap-2">
        <Button className="w-full" variant="primary" size="sm">
          Set up scheduled reports
        </Button>
        <Button className="w-full" variant="outline" size="sm">
          See what&apos;s new
        </Button>
      </CardFooter>
    </Card>
  );
}
