.class public Lcom/demo/repackage/ProxyHookPMS;
.super Ljava/lang/Object;
.source "ProxyHookPMS.java"

# interfaces
.implements Ljava/lang/reflect/InvocationHandler;


# instance fields
.field private SIGN:Ljava/lang/String;

.field private appPkgName:Ljava/lang/String;

.field private base:Ljava/lang/Object;


# direct methods
.method public constructor <init>(Ljava/lang/Object;Ljava/lang/String;Ljava/lang/String;I)V
    .locals 0

    .line 21
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    const-string p4, ""

    .line 19
    iput-object p4, p0, Lcom/demo/repackage/ProxyHookPMS;->appPkgName:Ljava/lang/String;

    .line 23
    :try_start_0
    iput-object p1, p0, Lcom/demo/repackage/ProxyHookPMS;->base:Ljava/lang/Object;

    .line 24
    iput-object p2, p0, Lcom/demo/repackage/ProxyHookPMS;->SIGN:Ljava/lang/String;

    .line 25
    iput-object p3, p0, Lcom/demo/repackage/ProxyHookPMS;->appPkgName:Ljava/lang/String;
    :try_end_0
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_0} :catch_0

    goto :goto_0

    :catch_0
    move-exception p1

    .line 27
    invoke-virtual {p1}, Ljava/lang/Exception;->printStackTrace()V

    :goto_0
    return-void
.end method


# virtual methods
.method public invoke(Ljava/lang/Object;Ljava/lang/reflect/Method;[Ljava/lang/Object;)Ljava/lang/Object;
    .locals 3
    .annotation system Ldalvik/annotation/Throws;
        value = {
            Ljava/lang/Throwable;
        }
    .end annotation

    .line 33
    invoke-virtual {p2}, Ljava/lang/reflect/Method;->getName()Ljava/lang/String;

    move-result-object p1

    const-string v0, "getPackageInfo"

    invoke-virtual {v0, p1}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result p1

    if-eqz p1, :cond_0

    const/4 p1, 0x0

    .line 34
    aget-object v0, p3, p1

    check-cast v0, Ljava/lang/String;

    const/4 v1, 0x1

    .line 35
    aget-object v1, p3, v1

    check-cast v1, Ljava/lang/Integer;

    .line 36
    invoke-virtual {v1}, Ljava/lang/Integer;->intValue()I

    move-result v1

    const/16 v2, 0x40

    if-ne v1, v2, :cond_0

    iget-object v1, p0, Lcom/demo/repackage/ProxyHookPMS;->appPkgName:Ljava/lang/String;

    invoke-virtual {v1, v0}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v0

    if-eqz v0, :cond_0

    .line 37
    new-instance v0, Landroid/content/pm/Signature;

    iget-object v1, p0, Lcom/demo/repackage/ProxyHookPMS;->SIGN:Ljava/lang/String;

    invoke-direct {v0, v1}, Landroid/content/pm/Signature;-><init>(Ljava/lang/String;)V

    .line 38
    iget-object v1, p0, Lcom/demo/repackage/ProxyHookPMS;->base:Ljava/lang/Object;

    invoke-virtual {p2, v1, p3}, Ljava/lang/reflect/Method;->invoke(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object p2

    check-cast p2, Landroid/content/pm/PackageInfo;

    .line 39
    iget-object p3, p2, Landroid/content/pm/PackageInfo;->signatures:[Landroid/content/pm/Signature;

    aput-object v0, p3, p1

    return-object p2

    .line 43
    :cond_0
    iget-object p1, p0, Lcom/demo/repackage/ProxyHookPMS;->base:Ljava/lang/Object;

    invoke-virtual {p2, p1, p3}, Ljava/lang/reflect/Method;->invoke(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object p1

    return-object p1
.end method
