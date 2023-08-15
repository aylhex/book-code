.class public Lcom/demo/repackage/SecondFragment;
.super Landroidx/fragment/app/Fragment;
.source "SecondFragment.java"


# instance fields
.field private binding:Lcom/demo/repackage/databinding/FragmentSecondBinding;


# direct methods
.method public constructor <init>()V
    .locals 0

    .line 14
    invoke-direct {p0}, Landroidx/fragment/app/Fragment;-><init>()V

    return-void
.end method


# virtual methods
.method public onCreateView(Landroid/view/LayoutInflater;Landroid/view/ViewGroup;Landroid/os/Bundle;)Landroid/view/View;
    .locals 0

    const/4 p3, 0x0

    .line 24
    invoke-static {p1, p2, p3}, Lcom/demo/repackage/databinding/FragmentSecondBinding;->inflate(Landroid/view/LayoutInflater;Landroid/view/ViewGroup;Z)Lcom/demo/repackage/databinding/FragmentSecondBinding;

    move-result-object p1

    iput-object p1, p0, Lcom/demo/repackage/SecondFragment;->binding:Lcom/demo/repackage/databinding/FragmentSecondBinding;

    .line 25
    invoke-virtual {p1}, Lcom/demo/repackage/databinding/FragmentSecondBinding;->getRoot()Landroidx/constraintlayout/widget/ConstraintLayout;

    move-result-object p1

    return-object p1
.end method

.method public onDestroyView()V
    .locals 1

    .line 43
    invoke-super {p0}, Landroidx/fragment/app/Fragment;->onDestroyView()V

    const/4 v0, 0x0

    .line 44
    iput-object v0, p0, Lcom/demo/repackage/SecondFragment;->binding:Lcom/demo/repackage/databinding/FragmentSecondBinding;

    return-void
.end method

.method public onViewCreated(Landroid/view/View;Landroid/os/Bundle;)V
    .locals 0

    .line 30
    invoke-super {p0, p1, p2}, Landroidx/fragment/app/Fragment;->onViewCreated(Landroid/view/View;Landroid/os/Bundle;)V

    .line 32
    iget-object p1, p0, Lcom/demo/repackage/SecondFragment;->binding:Lcom/demo/repackage/databinding/FragmentSecondBinding;

    iget-object p1, p1, Lcom/demo/repackage/databinding/FragmentSecondBinding;->buttonSecond:Landroid/widget/Button;

    new-instance p2, Lcom/demo/repackage/SecondFragment$1;

    invoke-direct {p2, p0}, Lcom/demo/repackage/SecondFragment$1;-><init>(Lcom/demo/repackage/SecondFragment;)V

    invoke-virtual {p1, p2}, Landroid/widget/Button;->setOnClickListener(Landroid/view/View$OnClickListener;)V

    return-void
.end method
