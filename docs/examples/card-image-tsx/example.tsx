import {
  Badge,
  Button,
  Card,
  CardAction,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Card className="relative w-full max-w-sm pt-0">
      <Widget className="relative h-[216px] w-full rounded-t-xl bg-slate-400">
        <Widget className="absolute left-0 top-0 h-full w-full bg-black/35" />
      </Widget>
      <CardHeader>
        <CardTitle>Design systems meetup</CardTitle>
        <CardDescription>
          A practical talk on component APIs, accessibility, and shipping
          faster.
        </CardDescription>
        <CardAction>
          <Badge variant="secondary">Featured</Badge>
        </CardAction>
      </CardHeader>
      <CardFooter>
        <Button className="w-full" variant="primary">
          View Event
        </Button>
      </CardFooter>
    </Card>
  );
}
