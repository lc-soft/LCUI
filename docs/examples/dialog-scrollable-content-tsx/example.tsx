import {
  Dialog,
  DialogTrigger,
  DialogOverlay,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  ScrollArea,
  ScrollAreaContent,
  Scrollbar,
  Text,
  Portal,
} from "@lcui/react";

export default function Example() {
  return (
    <Dialog>
      <DialogTrigger>Scrollable Content</DialogTrigger>
      <Portal>
        <DialogOverlay>
          <DialogContent>
            <DialogHeader>
              <DialogTitle>Scrollable Content</DialogTitle>
              <DialogDescription>
                This is a dialog with scrollable content.
              </DialogDescription>
            </DialogHeader>
            <ScrollArea className="h-50">
              <ScrollAreaContent>
                <Text>Lorem ipsum dolor sit amet, consectetur adipiscing elit.</Text>
                <Text>
                  Sed do eiusmod tempor incididunt ut labore et dolore magna
                  aliqua.
                </Text>
                <Text>
                  Ut enim ad minim veniam, quis nostrud exercitation ullamco
                  laboris.
                </Text>
                <Text>
                  Duis aute irure dolor in reprehenderit in voluptate velit
                  esse cillum dolore.
                </Text>
                <Text>
                  Excepteur sint occaecat cupidatat non proident, sunt in culpa
                  qui officia deserunt mollit anim id est laborum.
                </Text>
              </ScrollAreaContent>
              <Scrollbar />
            </ScrollArea>
          </DialogContent>
        </DialogOverlay>
      </Portal>
    </Dialog>
  );
}
