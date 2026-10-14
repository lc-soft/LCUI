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
  TextInput,
  Portal,
} from "@lcui/react";

export default function Example() {
  return (
    <Dialog>
      <DialogTrigger>Share</DialogTrigger>
      <Portal>
        <DialogOverlay>
          <DialogContent className="max-w-md">
            <DialogHeader>
              <DialogTitle>Share link</DialogTitle>
              <DialogDescription>
                Anyone who has this link will be able to view this.
              </DialogDescription>
            </DialogHeader>
            <TextInput
              defaultValue="https://lcui.dev/docs/installation"
              readOnly
              className="flex-1"
            />
            <DialogFooter className="justify-start">
              <DialogClose>Close</DialogClose>
            </DialogFooter>
            <DialogClose />
          </DialogContent>
        </DialogOverlay>
      </Portal>
    </Dialog>
  );
}
