import {
  Button,
  Card,
  CardContent,
  CardHeader,
  CardTitle,
  Collapsible,
  CollapsibleContent,
  CollapsibleTrigger,
  Icon,
  Text,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <Widget className="w-[320px]">
      <Card size="sm" className="w-full">
        <CardHeader>
          <CardTitle>Project files</CardTitle>
        </CardHeader>
        <CardContent>
          <Widget className="flex flex-col gap-0.5 pt-2">
            <Collapsible defaultOpen>
              <CollapsibleTrigger>
                <Button
                  variant="ghost"
                  size="sm"
                  className="w-full justify-start border-0 bg-transparent"
                >
                  <Icon name="chevron-right" />
                  <Icon name="folder-open" />
                  <Text>components</Text>
                </Button>
              </CollapsibleTrigger>
              <CollapsibleContent>
                <Widget className="ml-5 flex flex-col gap-0.5">
                  <Collapsible defaultOpen>
                    <CollapsibleTrigger>
                      <Button
                        variant="ghost"
                        size="sm"
                        className="w-full justify-start border-0 bg-transparent"
                      >
                        <Icon name="chevron-right" />
                        <Icon name="folder-open" />
                        <Text>ui</Text>
                      </Button>
                    </CollapsibleTrigger>
                    <CollapsibleContent>
                      <Widget className="ml-5 flex flex-col gap-0.5">
                        {["button.tsx", "card.tsx", "dialog.tsx"].map((name) => (
                          <Button
                            key={name}
                            variant="link"
                            size="sm"
                            className="w-full justify-start"
                          >
                            <Icon name="document-text" />
                            <Text>{name}</Text>
                          </Button>
                        ))}
                      </Widget>
                    </CollapsibleContent>
                  </Collapsible>
                  {["login-form.tsx", "register-form.tsx"].map((name) => (
                    <Button
                      key={name}
                      variant="link"
                      size="sm"
                      className="w-full justify-start"
                    >
                      <Icon name="document-text" />
                      <Text>{name}</Text>
                    </Button>
                  ))}
                </Widget>
              </CollapsibleContent>
            </Collapsible>
            <Collapsible>
              <CollapsibleTrigger>
                <Button
                  variant="ghost"
                  size="sm"
                  className="w-full justify-start border-0 bg-transparent"
                >
                  <Icon name="chevron-right" />
                  <Icon name="folder-open" />
                  <Text>lib</Text>
                </Button>
              </CollapsibleTrigger>
              <CollapsibleContent>
                <Widget className="ml-5 flex flex-col gap-0.5">
                  {["utils.ts", "api.ts"].map((name) => (
                    <Button
                      key={name}
                      variant="link"
                      size="sm"
                      className="w-full justify-start"
                    >
                      <Icon name="document-text" />
                      <Text>{name}</Text>
                    </Button>
                  ))}
                </Widget>
              </CollapsibleContent>
            </Collapsible>
            {["app.tsx", "package.json", "README.md"].map((name) => (
              <Button
                key={name}
                variant="link"
                size="sm"
                className="w-full justify-start"
              >
                <Icon name="document-text" />
                <Text>{name}</Text>
              </Button>
            ))}
          </Widget>
        </CardContent>
      </Card>
    </Widget>
  );
}
