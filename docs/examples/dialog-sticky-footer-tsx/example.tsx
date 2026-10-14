import {
  Dialog,
  DialogTrigger,
  DialogOverlay,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  DialogFooter,
  DialogClose,
  ScrollArea,
  ScrollAreaContent,
  Scrollbar,
  Text,
  Portal,
} from "@lcui/react";

export default function Example() {
  return (
    <Dialog>
      <DialogTrigger>Sticky Footer</DialogTrigger>
      <Portal>
        <DialogOverlay>
          <DialogContent>
            <DialogHeader>
              <DialogTitle>Sticky Footer</DialogTitle>
              <DialogDescription>
                This dialog has a sticky footer that stays visible while the
                content scrolls.
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
            <DialogFooter>
              <DialogClose variant="outline">Close</DialogClose>
            </DialogFooter>
          </DialogContent>
        </DialogOverlay>
      </Portal>
    </Dialog>
  );
}
